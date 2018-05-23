#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

// duplicates any add instruction, the new add is inserted before the old one
// test bitcode is provided in hello.bc, compiled with clang -O0 from hello.c
// disassembled bitcode can be found in hello.ll

using namespace llvm;

bool runOnBasicBlock(BasicBlock &BB) {
	bool modified = false;
  for(BasicBlock::iterator Inst = BB.begin(), E = BB.end(); Inst != E; ++Inst) {
      if (!Inst->isBinaryOp())
      	continue;
      if (Inst->getOpcode() != Instruction::Add)
      	continue;
      errs() << "Found one!\n";
      Value* op1 = Inst->getOperand(0);
      Value* op2 = Inst->getOperand(1);
      op1->printAsOperand(errs());
      errs() << "\n";
      op2->printAsOperand(errs());
      errs() << "\n";
      BinaryOperator *NewVal = BinaryOperator::Create(Instruction::Add, op1, op2);
      NewVal->insertBefore(&(*Inst));
      modified = true;
  }
  errs() << "Hello!";
  return modified;
}

int main()
{
  printf("Hello World!\n");
  LLVMContext context;
  SMDiagnostic error;
  auto m = parseIRFile("hello.bc", error, context);
  bool modified = false;
  if (!m)
  {
    printf("Error loading file hello.bc!\n");
    return -1;
  }
  for (auto functionIter = m->getFunctionList().begin(); functionIter != m->getFunctionList().end(); functionIter++) {
    printf("\tFunction %s has %i basic blocks\n", functionIter->getName().data(), (int)functionIter->getBasicBlockList().size());

    for (auto basicBlockIter = functionIter->getBasicBlockList().begin(); basicBlockIter != functionIter->getBasicBlockList().end(); basicBlockIter++) {
      printf("\t\tBasic block has %i instructions\n", (int)basicBlockIter->size());
      modified |= runOnBasicBlock(*basicBlockIter);
    }
  }
  if (modified)
  { 
    std::error_code EC;
    raw_fd_ostream toyBcOutput("hello.out.bc", EC, sys::fs::F_RW | sys::fs::F_Text);
    WriteBitcodeToFile(m.get(), toyBcOutput);
    toyBcOutput.close();
  }
  return 0;
}