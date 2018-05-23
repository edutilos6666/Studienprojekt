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

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace llvm;

bool parse(const std::string& FileName) {
  std::ifstream ifs(FileName);
  if (!ifs.is_open()) {
    printf("[!] cannot open '%s'\n", FileName.c_str());
    return false;
  }
  printf("[+] opened ASM file '%s'\n", FileName.c_str());
  
  // parsing 

  ifs.close();
  printf("[+] closed ASM file '%s'\n", FileName.c_str());

  // lifting 
  
  
  // create LLVM context and module with function main
  LLVMContext ctx;
  Module mod("Demo", ctx);

  IRBuilder<> Builder(ctx);

  auto mainFunctionType = FunctionType::get(Type::getInt8Ty(ctx), {}, false);
  auto mainFunction = Function::Create(mainFunctionType, Function::ExternalLinkage, "main", &mod);

  auto basicBlock = BasicBlock::Create(ctx, "entry", mainFunction);
  Builder.SetInsertPoint(basicBlock);
  
  auto ret = Builder.CreateRet(ConstantInt::get(ctx, APInt(8, 13)));
  auto op = BinaryOperator::Create(Instruction::Add, ConstantInt::get(ctx, APInt(8, 13)), ConstantInt::get(ctx, APInt(8, 13)));
  op->insertBefore(ret);

  if (llvm::verifyFunction(*mainFunction)) {
    printf("[!] bitcode for ASM file '%s' is malformed.\n", FileName.c_str());
    mainFunction->eraseFromParent();
    return false;
  }

  // write LLVM context bitcode to file 
  std::error_code EC;
  auto OutputFileName = "../../src/DemoPass/w00pw00p.bc";
  llvm::raw_fd_ostream ofs(OutputFileName, EC, sys::fs::F_RW | sys::fs::F_Text);
  llvm::WriteBitcodeToFile(&mod, ofs);
  ofs.flush();
  printf("[+] wrote bitcode of ASM file '%s' to '%s'\n", FileName.c_str(), OutputFileName);
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    return -1;
  }

  if (!parse(std::string(argv[1])))
    return 0;

  return 0;
}