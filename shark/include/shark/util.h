#pragma once

#include <cstdio>
#include <string>

#include "llvm/ADT/SmallVector.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"

#define NL_MAGIC "\n"
#define ADDNL_MAGIC(str) str NL_MAGIC
#define PRINT_ERROR(fmt, ...) printf("ERROR: " ADDNL_MAGIC(fmt), ##__VA_ARGS__)
#define PRINT_INFO(fmt, ...) printf("INFO: " ADDNL_MAGIC(fmt), ##__VA_ARGS__)
#define PRINT_FIXME(fmt, ...) printf("FIXME: " ADDNL_MAGIC(fmt), ##__VA_ARGS__)

//! Add metadata (size, instruction code as string) to an instruction
//! @param[in,out] ctx Our current llvm context
//! @param[in] codeString The complete instruction as a string for the
//! backend
//! @param[in,out] instruction The instruction on which the metadata should be
//! appended
//! @param[in] instructionSize the instructions size in byte
//! @return Whenever the operation succeded or failed
inline bool appendMetadata(llvm::LLVMContext &ctx,
                           const std::string &codeString,
                           llvm::Instruction *instruction,
                           const int instructionSize,
                            int readRegSrc=  -1, //LHS Wert
                            int readRegDst=  -1, //RHS Wert 
                           const int writeRegDst= -1){ //RHS Wert

  if (instruction == nullptr) {
    PRINT_ERROR("Failed to append metadata: Instruction is not set.");
    return false;
  }

  llvm::SmallVector<llvm::Metadata *, 32> Ops;
  llvm::SmallVector<llvm::Metadata *, 16> OpsR;
  llvm::SmallVector<llvm::Metadata *, 16> OpsW;
  
  Ops.push_back(llvm::MDString::get(ctx, codeString));
  Ops.push_back(llvm::ConstantAsMetadata::get(
      llvm::ConstantInt::get(ctx, llvm::APInt(64, instructionSize, false))));
  
  auto node = llvm::MDTuple::get(ctx, Ops);

  instruction->setMetadata("instruction", node);
  
  //Initalisieren und Bereitstellen der Read/Write Register falls vorhanden (absolut nicht schön, aber funktional!)
    if(!(readRegSrc==-1) || !(readRegDst==-1)){
        if (readRegSrc>readRegDst && readRegDst!=-1){
            int hilfe= readRegSrc;
            readRegSrc=readRegDst;
            readRegDst=hilfe;
        }
      for(int i=0;i<readRegSrc;i++){
        OpsR.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 0, false))));
        }
      if(readRegSrc!=-1){OpsR.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 1, false))));}
      for(int i=readRegSrc+1;i<readRegDst;i++){
        OpsR.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 0, false))));
        }
      if(readRegDst!=-1){OpsR.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 1, false))));}
      for(int i=readRegDst+1;i<16;i++){
        OpsR.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 0, false))));
        }
      auto read = llvm::MDTuple::get(ctx, OpsR);
      instruction->setMetadata("read", read);
    }
    if(!(writeRegDst==-1)){
      for(int i=0;i<writeRegDst;i++){
        OpsW.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 0, false))));
        }
      OpsW.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 1, false))));
      for(int i=writeRegDst+1;i<16;i++){
        OpsW.push_back(llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(ctx, llvm::APInt(64, 0, false))));
        }
      auto write= llvm::MDTuple::get(ctx, OpsW);
      instruction->setMetadata("write", write);
    }


  return true;
}
