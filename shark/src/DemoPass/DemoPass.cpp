#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"

// duplicates any add instruction, the new add is inserted before the old one
// test bitcode is provided in hello.bc, compiled with clang -O0 from hello.c
// disassembled bitcode can be found in hello.ll
// to run:
// opt -load lib/libdemopass.so -Demo < ../src/DemoPass/hello.bc > mybc.bc
// disassemble bitcode with llvm-dis

using namespace llvm;

namespace {
struct Demo : public BasicBlockPass {
  static char ID;
  Demo() : BasicBlockPass(ID) {}

  bool runOnBasicBlock(BasicBlock &BB) override {
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
}; // end of struct Demo
}  // end of anonymous namespace

char Demo::ID = 0;
static RegisterPass<Demo> X("Demo", "Duplicate \"add\" instructions",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);