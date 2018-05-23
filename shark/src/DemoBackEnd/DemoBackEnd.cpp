#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

using namespace llvm;

int main()
{
  LLVMContext context;
  SMDiagnostic error;
  auto m = parseIRFile("hello.bc", error, context);
  if (!m) {
  	printf("[-] Could not open file: %s\n", error.getMessage());
  	return 0;
  }

  std::error_code EC;
  raw_fd_ostream toyAsmOutput("hello.out.asm", EC, sys::fs::F_RW | sys::fs::F_Text);
  if (toyAsmOutput.has_error())
  {
  	printf("[-] Could not open output file for writing\n");
  	return 0;
  }

  printf("Number of functions in module: %i\n", m->getFunctionList().size());

  for (auto functionIter = m->getFunctionList().begin(); functionIter != m->getFunctionList().end(); functionIter++) {
  	printf("\tFunction %s has %i basic blocks\n", functionIter->getName(), functionIter->getBasicBlockList().size());

  	for (auto basicBlockIter = functionIter->getBasicBlockList().begin(); basicBlockIter != functionIter->getBasicBlockList().end(); basicBlockIter++) {
  		printf("\t\tBasic block has %i instructions\n", basicBlockIter->size());

  		for (auto instructionIter = basicBlockIter->begin(); instructionIter != basicBlockIter->end(); instructionIter++) {
  			outs() << "\t\t\t" << *instructionIter << "\n";

  			if (!instructionIter->isBinaryOp() || instructionIter->getOpcode() != Instruction::Add)
  				continue;

  			toyAsmOutput << instructionIter->getOpcodeName() << " ";
  			instructionIter->getOperand(0)->printAsOperand(toyAsmOutput);
  			toyAsmOutput << ", ";
  			instructionIter->getOperand(1)->printAsOperand(toyAsmOutput);
  			toyAsmOutput << "\n";
  		}
  	}
  }

  toyAsmOutput.close();

  return 0;
}