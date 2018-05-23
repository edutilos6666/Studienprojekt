#include "shark/util.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

//! Grab instruction size from metadata
bool getInstructionSize(const Instruction &I, size_t &sizeOut) {
  const auto MD = I.getMetadata("instruction");
  if (!MD) {
    PRINT_ERROR(
        "Metadata for instruction '%s' from '%s' in module '%s' missing.\n",
        I.getOpcodeName(),
        (I.getFunction() ? I.getFunction()->getName().str().c_str()
                         : "unknown"),
        I.getModule()->getName().str().c_str());
    return false;
  }
  auto CAM = cast<ConstantAsMetadata>(MD->getOperand(1));
  if (!CAM) {
    PRINT_ERROR("Metadata for instruction '%s' from '%s' in module '%s' "
                "incomplete: Unknown operand size.\n",
                I.getOpcodeName(),
                (I.getFunction() ? I.getFunction()->getName().str().c_str()
                                 : "unknown"),
                I.getModule()->getName().str().c_str());
    return false;
  }
  auto CI = cast<ConstantInt>(CAM->getValue());
  if (!CI) {
    PRINT_ERROR("Metadata for instruction '%s' from '%s' in module '%s' "
                "incomplete: Invalid operand size.\n",
                I.getOpcodeName(),
                (I.getFunction() ? I.getFunction()->getName().str().c_str()
                                 : "unknown"),
                I.getModule()->getName().str().c_str());
    return false;
  }
  sizeOut = CI->getZExtValue();
  return true;
}

//! Analyze the size of the basic block
bool analyzeBlockSize(const BasicBlock &BB, size_t &blocksizeOut) {
  blocksizeOut = 0;
  for (const auto &I : BB) {
    size_t curBlocksize;
    if (!getInstructionSize(I, curBlocksize)) {
      return false;
    }
    blocksizeOut += curBlocksize;
  }
  return true;
}

//! Splits the given block into multiple blocks of size below a given threshold
//! @return true if the block has been splitten, false otherwise
bool continuousSplitting(BasicBlock &BB, const size_t threshold) {
  size_t curSize = 0;
  size_t bbSize;
  if (!analyzeBlockSize(BB, bbSize)) {
    // Shit has hitten the fan when we get into this case... The previous
    // analysis pass has scanned all basic blocks and should have detected any
    // corrupt block. So we know, that we MUST have corrupted the basic block
    // somehow by ourselfs, thus we cannot proceed in any way and need to jump
    // out straight.
    PRINT_ERROR("Corrupted basic block found while splitting. Canceling!");
    exit(-2);
  }
  for (auto &I : BB) {
    size_t instSize;
    if (!getInstructionSize(I, instSize)) {
      // Look at the comment above...
      PRINT_ERROR("Corrupted instruction found while splitting. Canceling!");
      exit(-3);
    }
    curSize += instSize;
    if (curSize > threshold && bbSize != curSize) {
      auto nextBB = BB.splitBasicBlock(&I);
      auto &lastInst = *(BB.rbegin());
      PRINT_FIXME("embed br instruction correctly into metadata");
      appendMetadata(BB.getContext(), "br", &lastInst, 1);
      continuousSplitting(*nextBB, threshold);
      return true;
    }
  }
  return false;
}

#include <string>
int main(int argc, const char **argv) {

  cl::opt<std::string> InputFilename(cl::Positional, cl::Required,
                                     cl::desc("<input file>"));
  cl::opt<std::string> OutputFilename(cl::Positional, cl::Required,
                                      cl::desc("<output file>"));
  cl::ParseCommandLineOptions(argc, argv);

  LLVMContext context;
  SMDiagnostic error;

  auto M = parseIRFile(InputFilename.c_str(), error, context);
  if (!M) {
    PRINT_ERROR("Error loading file '%s'\n", InputFilename.c_str());
    return -1;
  }

  size_t totalInstructionSize = 0;
  size_t numBB = 0;

  // analysis
  {
    PRINT_INFO("Starting analysis pass.");
    for (auto &F : M->getFunctionList()) {
      for (auto &BB : F.getBasicBlockList()) {
        size_t blocksize;
        if (!analyzeBlockSize(BB, blocksize)) {
          PRINT_ERROR("Failed to analyze a basic block in function '%s'\n",
                      F.getName().str().c_str());
          return -1;
        }
        totalInstructionSize += blocksize;
        numBB++;
      }
    }
    PRINT_INFO("Finished analysis pass.");
  }
  const size_t averageBBSize =
      totalInstructionSize / ((numBB != 0) ? numBB : 1);

  // splitting
  PRINT_INFO("Starting basic block normalization with size %zu byte.",
             averageBBSize);
  {
    PRINT_INFO("Starting splitting pass.");
    for (auto &F : M->getFunctionList()) {
      auto BB = F.getBasicBlockList().begin();
      while (BB != F.getBasicBlockList().end()) {
        // as splitting invalidates the iterator, we need to reset it
        // every time it modifies F
        if (continuousSplitting(*BB, averageBBSize)) {
          BB = F.getBasicBlockList().begin();
        } else {
          BB++;
        }
      }
    }
    PRINT_INFO("Finished splitting pass");
  }

  // BB padding
  {
    PRINT_INFO("Starting padding pass.");
    size_t padding = 0;
    std::string codeString = "NOP";
    size_t BBSize;
    for (auto &F : M->getFunctionList()) {
      auto BB = F.getBasicBlockList().begin();
      while (BB != F.getBasicBlockList().end()) {
        analyzeBlockSize(*BB, BBSize);
        if (BBSize < averageBBSize) {
          padding = averageBBSize - BBSize;
          auto iterator = BB->begin();

          Value *v = ConstantInt::get(Type::getInt1Ty(context), 0, true);
          auto inst = BinaryOperator::Create(Instruction::Add, v, v, codeString,
                                             &(*iterator));
          // nop in die Metadaten aufnehmen
          if (!appendMetadata(context, codeString, inst, padding)) {
            PRINT_ERROR("Failed to append nop, due to corrupted instruction.");
            exit(-4);
          }
        }
        BB++;
      }
    }
    PRINT_INFO("Finished padding pass.");
  }
  {
    PRINT_INFO("Starting Def-Use analysis pass.");

    size_t numberOfBBs = 0;
    for (auto &F : M->getFunctionList()) {
      for (auto &BB : F.getBasicBlockList()) {
        numberOfBBs++;
      }
    }
    int b = -1;
    // std::array<bool, 16> UpExp;
    // std::array<bool, 16> VarKill;
    std::vector<std::array<bool, 16>> UpExp(numberOfBBs);
    std::vector<std::array<bool, 16>> VarKill(numberOfBBs);

    for (auto &F : M->getFunctionList()) {
      for (auto &BB : F.getBasicBlockList()) {
        bool firstInstruction = true;
        for (auto &I : BB) {
          if (firstInstruction) {
            b++;
            UpExp[b] = {};
            VarKill[b] = {};
            firstInstruction = false;
          }
          auto metaRead = I.getMetadata("read");
          if (metaRead) {
            for (int i = 0; i < 16; i++) {
              auto CAMread = cast<ConstantAsMetadata>(metaRead->getOperand(i));
              auto CIread = cast<ConstantInt>(CAMread->getValue());
              bool read = CIread->getZExtValue();
              if (read == VarKill[b][i]) {
                UpExp[b][i] = UpExp[b][i] || read;
              }
            }
          }
          auto metaWrite = I.getMetadata("write");
          if (metaWrite) {
            for (int i = 0; i < 16; i++) {
              auto CAMwrite =
                  cast<ConstantAsMetadata>(metaWrite->getOperand(i));
              auto CIwrite = cast<ConstantInt>(CAMwrite->getValue());
              bool write = CIwrite->getZExtValue();
              VarKill[b][i] = VarKill[b][i] || write;
            }
          }
        }
      }
    }
    PRINT_INFO("Finished Def-Use analysis pass.");
  }
  {
    PRINT_INFO("Saving results back to %s.", OutputFilename.c_str());
    std::error_code EC;
    raw_fd_ostream toyBcOutput(OutputFilename.c_str(), EC,
                               sys::fs::F_RW | sys::fs::F_Text);
    WriteBitcodeToFile(M.get(), toyBcOutput);
    toyBcOutput.close();
  }
  return 0;
}
