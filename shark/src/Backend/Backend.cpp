#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "llvm/IR/Constants.h"
#include <map>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

using namespace llvm;


bool generateASM(const std::string &FileName);

void replaceAll(std::string &s, const std::string &search, const std::string &replace);

int getBBCount(std::unique_ptr<Module> &m);

void generateRandomCodeForNOP(int numberOfIRForInstr, raw_fd_ostream& asmOutput, std::string& indent);
std::pair<std::string,int> generateRandomRegAndValue();
int randNum(int min , int max);
/*
 * main entry point.
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return -1;
    }

    if (!generateASM(std::string(argv[1])))
        return 0;

    return 0;
}

/*
 * Reads .bc file content and based on metadata named "instruction" generates .asm file
 * for "nop" "add i16 %r0, 0" is repeated for the instruction size times (the second
 * operand in the metadata. (padding functionality).
 */
bool generateASM(const std::string &FileName) {
    LLVMContext context;
    SMDiagnostic error;
    auto m = parseIRFile(FileName, error, context);
    if (!m) {
        printf("[-] Could not open file: %s\n", error.getMessage());
        return 0;
    }

    std::error_code EC;
    std::string outputFileName = FileName;
    replaceAll(outputFileName, ".bc", ".asm");
    raw_fd_ostream asmOutput(outputFileName, EC, sys::fs::F_RW | sys::fs::F_Text);
    if (asmOutput.has_error()) {
        printf("[-] Could not open output file for writing\n");
        return 0;
    }

    std::string moduleName = m->getName().str();
    std::replace(moduleName.begin(), moduleName.end(), '/', '_');
    std::replace(moduleName.begin(), moduleName.end(), '.', 'x');
    std::replace(moduleName.begin(), moduleName.end(), '-', 'X');
    std::string indent = "    ";
    std::string nopInstr = "add i16 %r0, 0";
    int bbNumber = 1;
    int bbCount = getBBCount(m);

    /*
     * reading  global symbols
     * Note: That's not been implemented in Frontend yet.
     */
    // .global <<string>>
    NamedMDNode* globalNode = m->getNamedMetadata("global");
    if(globalNode) {
        std::string globalTxt =  cast<MDString>(cast<MDTuple>(globalNode->getOperand(0))->getOperand(0))->getString();
        asmOutput << globalTxt << "\n";
    }

    // .text
    NamedMDNode* textNode = m->getNamedMetadata("text");
    if(textNode) {
        std::string textTxt = cast<MDString>(cast<MDTuple>(textNode->getOperand(0))->getOperand(0))->getString();
        asmOutput << textTxt << "\n";
    }

    // .retain
    NamedMDNode* retainNode = m->getNamedMetadata("retain");
    if(retainNode) {
        std::string retainTxt = cast<MDString>(cast<MDTuple>(retainNode->getOperand(0))->getOperand(0))->getString();
        asmOutput << retainTxt << "\n";
    }

    // .retainrefs
    NamedMDNode* retainrefsNode = m->getNamedMetadata("retainrefs");
    if(retainrefsNode) {
        std::string retainrefsTxt = cast<MDString>(cast<MDTuple>(retainrefsNode->getOperand(0))->getOperand(0))->getString();
        asmOutput << retainrefsTxt << "\n";
    }

    // .cdecls C,LIST,"msp430.h"
    NamedMDNode* cdeclsNode = m->getNamedMetadata("retainrefs");
    if(cdeclsNode) {
        std::string cdeclsTxt = cast<MDString>(cast<MDTuple>(cdeclsNode->getOperand(0))->getOperand(0))->getString();
        asmOutput << cdeclsTxt << "\n";
    }



    for (auto functionIter = m->getFunctionList().begin(); functionIter != m->getFunctionList().end(); functionIter++) {

        for (auto basicBlockIter = functionIter->getBasicBlockList().begin();
             basicBlockIter != functionIter->getBasicBlockList().end(); basicBlockIter++) {
            std::string bbName = moduleName + "_" + std::to_string(bbNumber) + ":";
            std::string next_bbName = moduleName + "_" + std::to_string(bbNumber + 1);
            asmOutput << bbName << "\n";

            for (auto instructionIter = basicBlockIter->begin();
                 instructionIter != basicBlockIter->end(); instructionIter++) {
                MDNode *MD = instructionIter->getMetadata("instruction");
                std::string opcodeName = instructionIter->getOpcodeName();
                if (opcodeName == "br" && bbNumber < bbCount) {
                    asmOutput << indent << "jmp " << next_bbName << "\n";
                }
                if (MD) {
                    std::string nativeInstr;
                    int64_t numberOfIRForInstr;
                    std::string ellipsis;
                    auto numOps = MD->getNumOperands();
                    //number of operands must be 2
                    //outs() << "# of operands = " << numOps << "\n";
                    //if(numOps != 2) continue ;
                    nativeInstr = cast<MDString>(MD->getOperand(0))->getString();
                    outs() << nativeInstr << "\n";
                    //remove unnecessary jmps
                    if(nativeInstr.rfind("jmp", 0) == 0) {
                        outs() << "True" << "\n";
                        continue;
                    }
                    auto constant = cast<ConstantAsMetadata>(MD->getOperand(1))->getValue();
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(constant)) {
                        numberOfIRForInstr = CI->getSExtValue();
                    }

                    if (nativeInstr == "NOP") {
                     /*   while (numberOfIRForInstr > 0) {
                            --numberOfIRForInstr;
                            asmOutput << indent << nopInstr << "\n";
                        }*/
                        generateRandomCodeForNOP(numberOfIRForInstr, asmOutput, indent);
                    } else {
                        asmOutput << indent << nativeInstr << "\n";
                    }

                }





            }
            ++bbNumber;
        }
    }

    asmOutput.close();

    return true;
}


/*
 * Generates random code for NOP
 * I use here : add.w , sub.w and if numberOfIRForInstr is odd  at the end
 * I add cmp.w reg1, reg2 .
 */
void generateRandomCodeForNOP(int numberOfIRForInstr, raw_fd_ostream& asmOutput, std::string& indent) {
    int count = numberOfIRForInstr / 2;
    std::map<std::string , int> regsHolder;
    for(int i= 0 ; i< count; ++i) {
        regsHolder.insert(generateRandomRegAndValue());
    }
    for(auto it = regsHolder.begin(); it != regsHolder.end(); ++it) {
        std::string reg = it->first;
        int randValue = it->second;
        std::stringstream ssInstr;
        ssInstr << "add " << "#" << randValue << ", " << reg;
        asmOutput << indent << ssInstr.str() << "\n";
    }

    for(auto it = regsHolder.begin(); it != regsHolder.end(); ++it) {
        std::string reg = it->first;
        int randValue = it->second;
        std::stringstream ssInstr;
        ssInstr << "sub " << "#" << randValue << ", " << reg;
        asmOutput << indent <<  ssInstr.str() << "\n";
    }

    if(numberOfIRForInstr%2 != 0) {
        std::stringstream reg1, reg2;
        reg1 << "r" << randNum(0, 15);
        reg2 << "r" << randNum(0 ,15);
        asmOutput << indent << "cmp " << reg1.str() << ", " << reg2.str() << "\n";
    }
}

std::pair<std::string,int> generateRandomRegAndValue() {
    int regNum = randNum(0, 15);
    std::stringstream ssReg;
    ssReg << "r" << regNum;
    std::string reg = ssReg.str();

    int randValue = randNum(1, 255);
    return std::pair<std::string,int>(reg, randValue);
}

int randNum(int min , int max) {
    return (rand()% (max-min+1)) + min;
}



/*
 * Replaces all occurrences of search string with replace string.
 * This function is used in order to construct output file name from input file name.
 */
void replaceAll(std::string &s, const std::string &search, const std::string &replace) {
    for (size_t pos = 0;; pos += replace.length()) {
        // Locate the substring to replace
        pos = s.find(search, pos);
        if (pos == std::string::npos) break;
        // Replace by erasing and inserting
        s.erase(pos, search.length());
        s.insert(pos, replace);
    }
}

/*
 * Counts and returns basic block counts inside function.
 */
int getBBCount(std::unique_ptr<Module> &m) {
    int bbCount = 0;
    for (auto functionIter = m->getFunctionList().begin(); functionIter != m->getFunctionList().end(); functionIter++) {
        bbCount += functionIter->getBasicBlockList().size();
    }
    return bbCount;
}