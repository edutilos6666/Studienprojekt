#include <string>
#include <fstream>

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include "llvm/IR/Metadata.h"
#include "llvm/IR/PassManager.h"
#include "shark/util.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

LLVMContext ctx;
Module mod("Demo", ctx);
IRBuilder<> Builder(ctx);
static std::map<std::string, Value*> RegisterMap;
static std::map<std::string, llvm::BasicBlock*> BasicBlockMap;


enum Token {
   tok_eof = -1,
   tok_op=-2,
   tok_op1=-3,
   tok_emop = -4,

   tok_head_gobal = -5,
   tok_head_text = -6,

   tok_label = -7,
   tok_jmp = -8
};


//Globale Metadaten werden an das Modul angehängt, indem an NamedMDNodes(Attribut vom Modul) wird ein neu erzeugtes MDTupel angehängt, welches in seinem ArrayRef hoffentlich nur einen einfachen MDString enthält
void appendGlobalMD(std::string identifier, std::string head){
    if(identifier=="global"){
      NamedMDNode* globalHead = mod.getOrInsertNamedMetadata("global");
      globalHead->addOperand(MDTuple::get(ctx,(ArrayRef<Metadata*>(MDString::get(ctx,".global "+head)))));
      errs()<<"Appending :GlobalMD";
    }else if(identifier=="text"){
      NamedMDNode* textHead = mod.getOrInsertNamedMetadata("text");
      textHead->addOperand(MDTuple::get(ctx,(ArrayRef<Metadata*>(MDString::get(ctx,".text ")))));
      errs()<<"Appending .TextMD";
    }
}

// LHS/RHS kürzen um die angesprochenen Register als Int an die Metadaten anzuhängen (nur für RegisterMode/IndexedMode/IndirectRegisterMode/IndirectAutoIncrementMode möglich)
 int registerToInt(std::string registerStr){
        if(isdigit(registerStr.front()))
            return -1;
        while(!isdigit(registerStr.front())){
            registerStr.erase(0,1);
        }

        std::string registerInt="0";
        while(isdigit(registerStr.front())){
            registerInt.push_back(registerStr.front());
            registerStr.erase(0,1);
        }
        return std::stoi(registerInt,nullptr);
    }



int getHeadInformations(std::ifstream* ifs, std::string* head, std::string* firstLabel){
  static int LastChar;
  //keine ahnung warum aber ohne den Try-Catch block
  //funktioniert das parsen von ',' nicht
  try{
    LastChar = ' ';
  }catch(const std::exception& e){
    errs()<<"Couldn't set LastChar to \' \'";
  }

  while(isspace(LastChar)){
    LastChar = ifs->get();
  }

  if(LastChar=='.'){
    std::string tmpHeadIdentifier;
    while(isalnum(LastChar = ifs->get())){
      tmpHeadIdentifier += LastChar;
    }
    if(tmpHeadIdentifier == "global"){
      std::string tmpHead;
      while(isalnum(LastChar = ifs->get())){
        tmpHead += LastChar;
      }
      *head = tmpHead;
      appendGlobalMD(tmpHeadIdentifier,tmpHead);
      return tok_head_gobal;
    }else if(tmpHeadIdentifier == "text"){
      appendGlobalMD(tmpHeadIdentifier,"");
      return tok_head_text;
    }
  }

  if(isalpha(LastChar)){
    std::string tmpLabel;
    tmpLabel+=LastChar;

    while(isalnum((LastChar = ifs->get())))
      tmpLabel+=LastChar;

      if(LastChar==':'){
        *firstLabel = tmpLabel;
        return tok_label;
      }
  }
}


//lexer
//get Instruction token (add, mul,...)
int gettok(std::ifstream* ifs, std::string *opcode, int *instructionSize){
  errs()<<"\nStart Parsing\n";

  static int LastChar;
  //keine ahnung warum aber ohne den Try-Catch block
  //funktioniert das parsen von ',' nicht
  try{
    LastChar = ' ';
  }catch(const std::exception& e){
    errs()<<"Couldn't set LastChar to \' \'";
  }

  while(isspace(LastChar)){
    LastChar = ifs->get();
  }

  //FIXME
  /*if(LastChar == ';'){
    errs()<<"comment\n";
    while(LastChar != EOF || LastChar != '\n' || LastChar != '\r'){
      LastChar = ifs->get();
    }
  }*/

  //read opcode
  std::string tmpOpcode;
  if(isalpha(LastChar)){
    tmpOpcode=LastChar;


    while(isalnum((LastChar = ifs->get())))
      tmpOpcode+=LastChar;

    if(LastChar == '.' || LastChar == ' ' || LastChar == '\n'){
      if(LastChar == '.'){
        LastChar = ifs->get();
        if(LastChar=='w')
          *instructionSize = 2;
        else if(LastChar=='b')
          *instructionSize = 1;
        else
          errs()<<"Missing Instruction Size";

      }else{
        *instructionSize = 2;
      }

      errs()<<"Found "+ tmpOpcode +" Instruction"+" with " + std::to_string(*instructionSize) +" Byte\n";
      //replace with switch for diffrent tokens (tok_op, tok_mull,...)
      if(tmpOpcode == "add" || tmpOpcode =="mov" || tmpOpcode =="sub" || tmpOpcode =="and" || tmpOpcode =="xor" || tmpOpcode=="bic" || tmpOpcode=="bis"
        /*||andere Befehle*/){
        *opcode = tmpOpcode;
        return tok_op;
      }else if(tmpOpcode == "push" /*||...*/){
        return tok_op1;
      //emulated Instructions
      }else if(tmpOpcode == "clr" || tmpOpcode == "dec" || tmpOpcode == "decd" || tmpOpcode == "inc" || tmpOpcode=="incd" || tmpOpcode == "inv" || tmpOpcode == "nop" ||
               tmpOpcode == "rla" || tmpOpcode == "ret"/*||...*/){
        *opcode = tmpOpcode;
        return tok_emop;
        //jump instructions
      }else if(tmpOpcode == "jmp" || tmpOpcode == "jl" || tmpOpcode == "jge" || tmpOpcode == "jn" || tmpOpcode == "jnc" || tmpOpcode == "jc" || tmpOpcode == "jne" ||
               tmpOpcode == "jnz" || tmpOpcode == "jeq" || tmpOpcode == "jz"){
        *opcode = tmpOpcode;
        return tok_jmp;
      }

    }else if(LastChar == ':'){
        *opcode = tmpOpcode;
        return tok_label;
    }
  }
}


//get token for for Operation (Register, Immediate,...)
bool gettok2op(std::ifstream *ifs, int *NumValue, std::string *actRegister, int *adressingMode){
  static int LastChar;

  try{
    LastChar = ' ';
  }catch(const std::exception& e){
    errs()<<"ERROR for ,";
  }

  while(isspace(LastChar)|| LastChar==','){
    LastChar = ifs->get();
  }

  //FIXME
  /*if(LastChar == ';'){
    errs()<<"comment\n";
    while(LastChar != EOF || LastChar != '\n' || LastChar != '\r'){
      LastChar = ifs->get();
    }
  }*/

  //Immediate
  if(LastChar=='#'){
    LastChar = ifs->get();
    if(isdigit(LastChar)){
      std::string NumStr;
      while(isdigit(LastChar)){
        NumStr+=LastChar;
        LastChar=ifs->get();
      }
      *NumValue = std::stoi(NumStr);
      *actRegister=NumStr;
      *adressingMode=6;
    }
  }

  //indexed Register mode
  if(isdigit(LastChar)){
    std::string NumStr;
    NumStr += LastChar;
    while(isdigit(LastChar=ifs->get())){
      NumStr+=LastChar;
    }
    if(LastChar=='('){
      LastChar = ifs->get();
      if(isalpha(LastChar)){
        std::string tmpReg;
        tmpReg = LastChar;
        while(isalnum(LastChar= ifs->get())){
          tmpReg+= LastChar;
        }
        *actRegister=tmpReg;
        if(LastChar==')'){
          *NumValue = std::stoi(NumStr);
          *adressingMode=1;
        }
        else{
          errs()<<"malformed";
          return 0;
        }
      }
    }else{
      errs()<<"malformed";
      return 0;
    }
  }

  //absolute
  if(LastChar == '&'){
    std::string tmpAbsolute;
    while(isalnum(LastChar=ifs->get())){
      tmpAbsolute+=LastChar;
    }
    *adressingMode=3;
    *actRegister = tmpAbsolute;
  }

  //register
  if(LastChar == 'R'|| LastChar == 'r'){
    std::string tmpReg;
    tmpReg = LastChar;
    while(isalnum(LastChar= ifs->get())){
      tmpReg+=LastChar;
    }
    errs()<<"Register: " + tmpReg;
    *actRegister=tmpReg;
    *adressingMode=0;
  }

  //symbolic
  if(isalpha(LastChar)){
    std::string tmpReg;
    tmpReg = LastChar;
    while(isalnum(LastChar= ifs->get())){
      tmpReg+=LastChar;
    }
    errs()<<tmpReg;
    *actRegister=tmpReg;
    *adressingMode=2;
  }

  //Indirect (+ Autoinkrement)
  if(LastChar=='@'){
    LastChar=ifs->get();
    std::string tmpReg;
    tmpReg="";
    if(LastChar=='R' || LastChar=='r')
      tmpReg = LastChar;
    else
      errs()<<"malformed indirect \n";
    while (isalnum(LastChar = ifs->get())){
      tmpReg+=LastChar;
    }
    if (tmpReg=="")
        errs()<<"malformed indirect \n";
    *actRegister=tmpReg;
    if (LastChar=='+')
        *adressingMode=5;
    else
        *adressingMode=4;
  }

  //comment
  if(LastChar == ';'){
    errs()<<"comment\n";
    do{
    LastChar = ifs->get();
  }while (LastChar != EOF || LastChar != '\n' || LastChar != '\r');

    /*if(!ifs->eof())
      gettok2op(ifs,NumValue, actRegister, adressingMode);*/
  }
}


//most of the parameters should be clear.
//we only need the adressing mode from RHS because this is the register which maybe be changed (depending on the Adressing Mode)
int liftBinRegister(std::string opcode,std::string LHS, std::string RHS,int instructionSize, BasicBlock *BB, int adressingMode,
                    std::string instructionForMetadata,Instruction *firstLLVMinstruction, bool isMetadataset){

  errs()<<"\nlifting \""+instructionForMetadata +"\"\n";
  Instruction::BinaryOps instruction;
  if(opcode=="add")
    instruction = Instruction::Add;
  else if(opcode == "sub")
    instruction= Instruction::Sub;
  else if(opcode == "and")
    instruction = Instruction::And;
  else if(opcode == "xor")
    instruction = Instruction::Xor;
  else if(opcode == "bis")
    instruction = Instruction::Or;
  else if(opcode == "bic"){
    //NAND Befehl -> entweder über mehrere llvm Instructions:
    //DST = XOR(0xFFFF, SRC) AND DST
    //oder über atomicrmw
  }else if(opcode == "mov"){
    if(adressingMode==1 || adressingMode==3){
      Value *indexedLHS = RegisterMap["act_LHS"];
      Value *indexedRHS = RegisterMap["act_RHS"];
      if(!indexedLHS || !indexedRHS){
        errs()<<"Could not load Register.\n";
        return 0;
      }
      auto op = Builder.CreateStore(indexedLHS, indexedRHS,false);

    }else if(adressingMode==0){
      //FIXME
      Value *registerLHS = RegisterMap["act_LHS"];
      if(!registerLHS){
        errs()<<"Could not load Register.\n";
        return 0;
      }
      auto op = BinaryOperator::Create(Instruction::Add, ConstantInt::get(ctx, APInt(instructionSize*8,0)),registerLHS ,RHS, BB);
      RegisterMap[RHS]=op;
      if(!isMetadataset){
        firstLLVMinstruction=op;
      }else if(adressingMode==2){

      }
      appendMetadata(ctx,instructionForMetadata,firstLLVMinstruction,instructionSize,registerToInt(LHS),-1,registerToInt(RHS));
    }
    return 1;
  }
  Value *v = RegisterMap["act_RHS"];
  if(!v){
    errs()<<"Could not load Register (RHS).\n";
    return 0;
  }
  Value *v2 = RegisterMap["act_LHS"];
  if(!v2){
    errs()<<"Could not load Register (LHS).";
    return 0;
  }

  if(adressingMode==1 || adressingMode == 3 || adressingMode==4 || adressingMode == 5){
    auto op = BinaryOperator::Create(instruction, v2,v,"TMP_1",BB);
    auto opStore = Builder.CreateStore(op, v,false);
  }else if(adressingMode==0){
    auto op = BinaryOperator::Create(instruction, v2,v,RHS,BB);
    RegisterMap[RHS]=op;
    if(!isMetadataset){
      firstLLVMinstruction=op;
    }
  }
  appendMetadata(ctx,instructionForMetadata,firstLLVMinstruction,instructionSize,registerToInt(LHS),registerToInt(RHS),registerToInt(RHS));
  return 1;
}


//lift emulated-Instructions
int liftEmulOp(std::string opcode,std::string DST, int instructionSize, BasicBlock *BB, int adressingMode,
               std::string instructionForMetadata,Instruction *firstLLVMinstruction, bool isMetadataset){

  errs()<<"\nlifting emulated instruction \""+instructionForMetadata +"\"\n";
  Instruction::BinaryOps instruction;
  Value *SRC;

  if(opcode == "clr"){
    SRC = ConstantInt::get(ctx, APInt(instructionSize*8,0));
    instruction = Instruction::Add;

    if(adressingMode==1 || adressingMode == 3){

      Value *v = RegisterMap["act_LHS"];
      if(!v){
        errs()<<"Could not load Register.";
        return 0;
      }
      auto op = BinaryOperator::Create(instruction, SRC,SRC,"TMP_1",BB);
      auto opStore = Builder.CreateStore(op, v,false);
    }else if(adressingMode==0){
      auto op = BinaryOperator::Create(instruction, SRC,SRC,DST,BB);
      RegisterMap[DST]=op;
      if(!isMetadataset){
        firstLLVMinstruction=op;
      }
    }
    if(appendMetadata(ctx,instructionForMetadata,firstLLVMinstruction,instructionSize,-1,-1,registerToInt(DST)))
      return true;
    else
      return false;

  }else if(opcode == "dec"){
    SRC = ConstantInt::get(ctx, APInt(instructionSize*8,1));
    instruction = Instruction::Sub;
  }else if(opcode == "decd"){
    SRC = ConstantInt::get(ctx, APInt(instructionSize*8,2));
    instruction = Instruction::Sub;
  }else if(opcode == "inc"){
    SRC = ConstantInt::get(ctx, APInt(instructionSize*8,1));
    instruction = Instruction::Add;
  }else if(opcode == "incd"){
    SRC = ConstantInt::get(ctx, APInt(instructionSize*8,2));
    instruction = Instruction::Add;
  }else if (opcode == "nop"){
    //TODO
  }else if(opcode == "ret"){
    auto ret = Builder.CreateRet(ConstantInt::get(ctx,APInt(16,0)));
    appendMetadata(ctx,instructionForMetadata,ret,2);
    return true;
  }

  Value *v = RegisterMap["act_LHS"];
  if(!v){
    errs()<<"Could not load Register.";
    return 0;
  }
  if(adressingMode==1 || adressingMode == 3){
    auto op = BinaryOperator::Create(instruction, SRC,v,"TMP_1",BB);
    auto opStore = Builder.CreateStore(op, v,false);
  }else if(adressingMode==0){
    auto op = BinaryOperator::Create(instruction, SRC,v,DST,BB);
    RegisterMap[DST]=op;
    if(!isMetadataset){
      firstLLVMinstruction=op;
    }
  }
  if(appendMetadata(ctx,instructionForMetadata,firstLLVMinstruction,instructionSize,-1,registerToInt(DST),registerToInt(DST)))
    return true;
  else
    return false;
}

int JumpLift(std::string opcode, std::string jumplabel, int instructionSize, llvm::BasicBlock *BB){
  errs()<<"Parsing " + opcode + " with Label " + jumplabel +"\n";
  std::string instructionForMetadata = opcode + " " + jumplabel;
  Instruction *firstLLVMinstruction;
  llvm::BasicBlock *jumpgoal = BasicBlockMap[jumplabel]; //read basicblock from map with jumplabel
  if(opcode == "jmp"){
    //unconditional jump
    errs()<<"unconditional jump \n";
    auto op = llvm::BranchInst::Create(jumpgoal, BB);
    errs()<<"jmp erstellt \n";
    firstLLVMinstruction = op;
  }//else if(opcode =="")
  if(appendMetadata(ctx, instructionForMetadata, firstLLVMinstruction, instructionSize)){
    errs()<<"Jump fertig, Metadaten weitergegeben";
    return true;
  }
  else
    return false;
}

//create additional llvm ir instructions for Indexed Adressing mode
void prepareIndexedAdressingmode(std::string regist, int registerOffset,int instructionSize, BasicBlock *BB, int side,
                                 std::string opcode,Instruction **firstLLVMinstruction, bool *isMetadataset){
  errs()<<"prepare Register "+regist+"\n";
  Value *tmp1 = RegisterMap[regist];
  Type* I;

  if(instructionSize==2)
    I = IntegerType::getInt16Ty(ctx);
  else if(instructionSize==1)
    I = IntegerType::getInt8Ty(ctx);


  auto op1 = BinaryOperator::Create(Instruction::Add, tmp1, ConstantInt::get(ctx, APInt(instructionSize*8, registerOffset)), regist+"_TMP_1", BB);

  if(!*isMetadataset){
    *firstLLVMinstruction=op1;
    *isMetadataset=true;
  }
  //da bei mov nur ein load nötig ist
  Instruction *op2;
  if(!(opcode =="mov" && side ==2))
    op2 = Builder.CreateLoad(I,op1,regist+"_TMP_2");
  else
    op2 = op1;

  if(side ==1)
    RegisterMap["act_LHS"]=op2;
  else if(side ==2)
    RegisterMap["act_RHS"]=op2;
}

//create additional llvm ir instructions for absolute Adressing mode
void prepareAbsoluteAdressingmode(std::string sourceadress, int instructionSize, BasicBlock *BB,int side, Instruction **firstLLVMinstruction, bool *isMetadataset){
  errs()<<"prepare Register "+sourceadress+"\n";
  Value *tmp1 = RegisterMap[sourceadress];
  Type* I;

  if(instructionSize==2)
    I = IntegerType::getInt16Ty(ctx);
  else if(instructionSize==1)
    I = IntegerType::getInt8Ty(ctx);

  //auto op1 = BinaryOperator::Create(Instruction::Add, tmp1, ConstantInt::get(ctx, APInt(instructionSize, 0)), "tmp1", BB);
  auto op1 = Builder.CreateLoad(I,tmp1,sourceadress+"_TMP_1");

  if(!*isMetadataset){
    *firstLLVMinstruction=op1;
    *isMetadataset=true;
  }

  if(side ==1){
    RegisterMap["act_LHS"]=op1;
  }else if(side == 2){
    RegisterMap["act_RHS"]=op1;
  }

    errs()<<*RegisterMap["act_RHS"];
}

//create additional llvm ir instructions for Indirect Adressing mode
void prepareIndirectAdressingmode(std:: string regist, int instructionSize, BasicBlock *BB, int side, std:: string opcode, Instruction **firstLLVMinstruction, bool *isMetadataset){
  errs()<<"prepare Register "+ regist + "\n";
  Value* temp = RegisterMap[regist];
  Type* I;

  if(instructionSize==2)
    I = IntegerType::getInt16Ty(ctx);
  else if(instructionSize==1)
    I = IntegerType::getInt8Ty(ctx);

  auto op1 = Builder.CreateLoad(I, temp, regist+"_TMP_1");

  if(!*isMetadataset){
    errs()<<"first instr \n";
    *firstLLVMinstruction = op1;
    *isMetadataset=true;
  }

  if (side==1){
    RegisterMap["act_LHS"] = op1;
    errs()<<"new lhs set \n";
  }else if(side==2){
    RegisterMap["act_RHS"] = op1;
    errs()<<"new rhs set \n";
  }
}

//create additional llvm ir instructions for Indirect Adressing mode with Inkrement
void prepareIndirectInkrementAdressingmode(std::string regist, int instructionSize, BasicBlock *BB, int side, std:: string opcode, Instruction **firstLLVMinstruction, bool *isMetadataset){
  errs()<<"prepare Register " + regist + "\n";
  Value* temp = RegisterMap[regist];
  Type* I;

  if(instructionSize==2)
    I = IntegerType::getInt16Ty(ctx);
  else if(instructionSize==1)
    I = IntegerType::getInt8Ty(ctx);

    auto op1 = Builder.CreateLoad(I, temp, regist+"_TMP_1");

  if(!*isMetadataset){
    errs()<<"first instr \n";
    *firstLLVMinstruction = op1;
    *isMetadataset=true;
  }

  if (side==1){
    RegisterMap["act_LHS"] = op1;
    errs()<<"new lhs set \n";
  }else if(side==2){
    RegisterMap["act_RHS"] = op1;
    errs()<<"new rhs set \n";
  }
  if (instructionSize==1){
    auto op2 = BinaryOperator::Create(Instruction::Add, temp, ConstantInt::get(ctx, APInt(instructionSize*8,1)), regist, BB);
    errs()<<"add set \n";
  }
  else if (instructionSize==2){
    auto op3 = BinaryOperator::Create(Instruction::Add, temp, ConstantInt::get(ctx, APInt(instructionSize*8,2)), regist, BB);
    errs()<<"add set \n";
  }
  RegisterMap[regist] = temp;
}
//void prepareAbsoluteAdressingmode(std::string regist, int instructionSize, BasicBlock *BB, BinaryOperator)
//main function for preparing the instructions before they actually lifted (e.g. create additional instructions)
//save the both registers which we need for the lift in the RegisterMap
bool prepareForLift(std::string opcode,std::string LHS,int LHSadressingMode, std::string RHS,int RHSadressingMode, int LHSoffset, int RHSoffset ,int instructionSize, BasicBlock *BB, bool oneOp){
  errs()<<"\nPreparing instruction\n";
  Value *tmp;
  std::string instructionForMetadata;
  instructionForMetadata+=opcode;

  Instruction *firstLLVMinstruction;
  bool isMetadataset=false;

  if(instructionSize==2)
    instructionForMetadata+=".w";
  else
    instructionForMetadata+=".b";

  //0...6 represents the diffrent adressing modes
  //0-> Register, 1->indexed, 2->symbolic, 3->absolute, 4->indirect, 5-> indirect autoincrement, 6->Immediate
  switch (LHSadressingMode) {
    case 0:
      instructionForMetadata+= " "+LHS;
      tmp = RegisterMap[LHS];
      RegisterMap["act_LHS"]=tmp;
      break;
    case 1:
      instructionForMetadata+= " "+std::to_string(LHSoffset)+"("+LHS+")";
      prepareIndexedAdressingmode(LHS, LHSoffset,instructionSize, BB,1,opcode,&firstLLVMinstruction,&isMetadataset); break;
    case 2:
      //FIXME
      instructionForMetadata+= " "+LHS;
      /*auto op1 = Builder.CreateLoad(I,tmp1,"tmp_1");
      RegisterMap["act_LHS"]=op1;
      firstLLVMinstruction = op1;*/
      errs()<<"This Addressing mode is temporarily not supported.";
      return false;
      break;
    case 3:
      instructionForMetadata+= " &"+LHS;
      prepareAbsoluteAdressingmode(LHS,instructionSize,BB,1,&firstLLVMinstruction,&isMetadataset);break;
    case 4:
      instructionForMetadata+= " @"+LHS;
      prepareIndirectAdressingmode(LHS, instructionSize, BB, 1, opcode, &firstLLVMinstruction, &isMetadataset); break;
    case 5:
      instructionForMetadata+= " @"+LHS;
      prepareIndirectInkrementAdressingmode(LHS, instructionSize, BB, 1, opcode, &firstLLVMinstruction, &isMetadataset); break;
    case 6:
      instructionForMetadata+=" #"+LHS;
      tmp = ConstantInt::get(ctx, APInt(instructionSize*8, std::stoi(LHS)));
      RegisterMap["act_LHS"]=tmp;
    default:
      break;
  }

  if(!oneOp){
    switch (RHSadressingMode) {
      case 0:
        instructionForMetadata+=", "+RHS;
        tmp = RegisterMap[RHS];
        RegisterMap["act_RHS"]=tmp;
        break;
      case 1:
        instructionForMetadata+=", "+std::to_string(RHSoffset)+"("+RHS+")";
        prepareIndexedAdressingmode(RHS,RHSoffset,instructionSize,BB,2,opcode,&firstLLVMinstruction,&isMetadataset); break;
      case 2:
        //FIXME
        //klappt noch nicht bzw. Lösung muss noch gefunden werden
        instructionForMetadata+=", "+RHS;
        errs()<<"This Addressing mode is temporarily not supported.";
        return false;
        break;
      case 3:
        instructionForMetadata+=", &"+RHS;
        prepareAbsoluteAdressingmode(RHS,instructionSize,BB,2,&firstLLVMinstruction,&isMetadataset);break;
      case 4:
        instructionForMetadata+= ", @"+RHS;
        prepareIndirectAdressingmode(RHS, instructionSize, BB, 2, opcode, &firstLLVMinstruction, &isMetadataset); break;
      case 5:
        instructionForMetadata+= ", @"+RHS+"+";
        prepareIndirectInkrementAdressingmode(RHS, instructionSize, BB, 2, opcode, &firstLLVMinstruction, &isMetadataset); break;
      default:
        break;
    }
    if(!liftBinRegister(opcode, LHS, RHS,instructionSize, BB, RHSadressingMode, instructionForMetadata,firstLLVMinstruction,isMetadataset)){
      errs()<<"Malformed Instruction.\nTerminating Program Now.\n";
      return false;
    }else{
      return true;
    }
  }else{
    if(!liftEmulOp(opcode, LHS, instructionSize, BB, LHSadressingMode, instructionForMetadata, firstLLVMinstruction, isMetadataset)){
      errs()<<"Malformed Instruction.\nTerminating Program Now.\n";
      return false;
    }else{
      return true;
    }
  }

  /*std::string opcode,std::string DST, int instructionSize, BasicBlock *BB, int adressingMode,
                 std::string instructionForMetadata,Instruction *firstLLVMinstruction, bool isMetadataset){*/
}



//main parse and lift function
bool parse(const std::string& FileName, const std::string& OutputName) {
  std::ifstream ifs(FileName);

  if (!ifs.is_open()) {
    printf("[!] cannot open '%s'\n", FileName.c_str());
    return false;
  }
  printf("[+] opened ASM file '%s'\n", FileName.c_str());


  std::string entryPoint = "not_Defined";
  std::string firstLabel = "not_Defined";
  int tok=0;

  //parsing head
  while(tok = getHeadInformations(&ifs, &entryPoint, &firstLabel)){
    if(tok == tok_head_gobal){
      errs()<<"\nFound Entry Point " + entryPoint+"\n";
    }else if(tok== tok_head_text){
      errs()<<"\nProgramm start.\n";
    }else if(tok == tok_label){
      errs()<<"\nFirst Label found. \nStart parsing MSP430 instructions\n";
      break;
    }

  }

  auto mainFunctionType = FunctionType::get(Type::getInt16Ty(ctx), {}, false);
  auto mainFunction = Function::Create(mainFunctionType, Function::ExternalLinkage, entryPoint, &mod);

  auto firstBB = BasicBlock::Create(ctx, firstLabel, mainFunction);
  BasicBlockMap[firstLabel] = firstBB;

  std::fpos<std::char_traits<char>::state_type> postheaderpos = ifs.tellg(); //???

  if (!ifs.is_open()) {
    printf("[!] cannot open '%s'to filter for labels \n", FileName.c_str());
    return false;
  }
  printf("[+] opened ASM file '%s' to filter for labels \n", FileName.c_str());

  int CurTok, CurTok2op, instructionSize, adressingMode;
  int NumValue =0;
  int LHSoffset =0;
  int RHSoffset=0;
  std::string opcode, actRegister, jumplabel;
  //find all labels and add them to the map
  while(CurTok=gettok(&ifs, &opcode, &instructionSize)){
    errs()<<".round of label \n";
    if(CurTok==tok_eof){
      errs()<<"eof \n";
      break;
    }
    if(CurTok==tok_label){
      auto basicBlock = BasicBlock::Create(ctx, opcode, mainFunction);
      BasicBlockMap[opcode] = basicBlock;
      errs()<<"found basic block with "+ opcode +"\n";
      //Builder.SetInsertPoint(basicBlock);
      CurTok=gettok(&ifs, &opcode, &instructionSize);
    }

    //two operands
    if(CurTok==tok_op){
      errs()<<"op \n";
      gettok2op(&ifs, &NumValue, &actRegister,&adressingMode); //durchlaufe register
      gettok2op(&ifs, &NumValue, &actRegister,&adressingMode);
    //one operand
    }else if(CurTok==tok_op1){
      errs()<<"single operand op \n";
      //TODO: register durchlaufen
    }else if(CurTok == tok_emop){ //emulated instructions (one instruction consisting of several instructions)
      errs()<<"emop \n";
      if(opcode!="ret")
        gettok2op(&ifs, &NumValue, &actRegister,&adressingMode);  //register durchlaufen
    }else if(CurTok == tok_jmp){
      errs()<<"jmp \n";
      //Durchlaufe das Label zum Jumpbefehl
      jumplabel = "";
      static int LastChar=ifs.get();
      while(isspace(LastChar)){
        LastChar=ifs.get();
      }
      while(isalnum(LastChar)){
        jumplabel+=LastChar;
        LastChar=ifs.get();
      }
      if(!isspace(LastChar)){
        errs()<<"wrong label format";
        return false;
      }
    }else{
      errs()<<"something might have gone wrong";
    }
  }

  ifs.clear();
  ifs.seekg(postheaderpos);
  errs()<<"Done filtering labels \n";

  /*auto basicBlock = BasicBlock::Create(ctx, firstLabel, mainFunction);
  BasicBlockMap[firstLabel]=basicBlock;*/
  auto basicBlock=BasicBlockMap[firstLabel];
  Builder.SetInsertPoint(basicBlock);



  //parsing Instructions
  while(CurTok=gettok(&ifs, &opcode, &instructionSize)){
    if(CurTok==tok_eof)
      break;

    if(CurTok==tok_label){
      /*basicBlock = BasicBlock::Create(ctx, opcode, mainFunction);
      BasicBlockMap[opcode]=basicBlock;*/
      basicBlock = BasicBlockMap[opcode];
      errs()<<"written basic block with "+opcode +"\n";
      Builder.SetInsertPoint(basicBlock);
      CurTok=gettok(&ifs, &opcode, &instructionSize);
    }

    bool twoRegs;
    //0...6 represents the diffrent adressing modes
    //0-> Register, 1->indexed, 2->symbolic, 3->absolute, 4->indirect, 5-> indirect autoincrement, 6->Immediate
    int LHSadressingMode;
    int RHSadressingMode;

    //two operands
    if(CurTok==tok_op){
      std::string LHS;
      std::string RHS;
      gettok2op(&ifs, &NumValue, &actRegister,&adressingMode);
      //save LHS Values
      LHS=actRegister;
      LHSadressingMode=adressingMode;
      LHSoffset=NumValue;
      //parsing instruction operands

      gettok2op(&ifs, &NumValue, &actRegister,&adressingMode);
      //save RHS Values
      RHS=actRegister;
      RHSoffset=NumValue;
      RHSadressingMode=adressingMode;

      if(!prepareForLift(opcode,LHS,LHSadressingMode, RHS, RHSadressingMode,LHSoffset, RHSoffset , instructionSize, basicBlock, false))
        return false;


    //one operand
    }else if(CurTok==tok_op1){
      //TODO
    }else if(CurTok == tok_emop){ //emulated instructions (one instruction consisting of several instructions)

      if(opcode == "ret"){
        if(!prepareForLift(opcode,"",0, "NULL", -1, 0, 0, instructionSize, basicBlock, true))
          return false;
      }else{
        gettok2op(&ifs, &NumValue, &actRegister, &adressingMode);

        std::string DST = actRegister;
        int offset = NumValue;


        if(!prepareForLift(opcode,DST,adressingMode, "NULL", -1, offset, 0, instructionSize, basicBlock, true))
          return false;
      }
    }else if(CurTok == tok_jmp){
        jumplabel = "";
        int counter = 0;
        static int LastChar=ifs.get();
        while(isspace(LastChar)){
            LastChar=ifs.get();
        }
        while(isalnum(LastChar)){
              jumplabel+=LastChar;
              LastChar=ifs.get();
              counter+=1;
              errs()<<std::to_string(counter) + " - ";
        }
        if(!isspace(LastChar)){
          errs()<<"wrong label format\n";
          return false;
        }
        JumpLift(opcode, jumplabel, instructionSize, basicBlock);
    }else{
      errs()<<"ERROR\nUnknown Instruction\n\n";
      return false;
    }

  }
  errs()<<"No instruction found\n";
  //auto ret = Builder.CreateRet(ConstantInt::get(ctx,APInt(16,0)));

  ifs.close();
  printf("\n[+] closed ASM file '%s'\n", FileName.c_str());
  errs()<<mod;
  if (llvm::verifyFunction(*mainFunction)) {
    printf("[!] bitcode for ASM file '%s' is malformed.\n", FileName.c_str());
    mainFunction->eraseFromParent();
    return false;
  }


  // write LLVM context bitcode to file
  std::error_code EC;
  auto OutputFileName = "./"+OutputName;
  llvm::raw_fd_ostream ofs(OutputFileName, EC, sys::fs::F_RW | sys::fs::F_Text);
  llvm::WriteBitcodeToFile(&mod, ofs);
  ofs.flush();
  printf("[+] wrote bitcode of ASM file '%s' to '%s'\n", FileName.c_str(), OutputFileName.c_str());
  return true;
}



int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s filename output-filename\n", argv[0]);
    return -1;
  }

  if (!parse(std::string(argv[1]), std::string(argv[2])))
    return 0;

  return 0;
}
