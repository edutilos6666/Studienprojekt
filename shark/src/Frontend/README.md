# Frontend

Lift MSP430 assembly to LLVM IR.

## Usage

Call the programm from the command line.
The general structure is:

  ./Frontend <"first-argument"> <"second-arguement">

The first input argument is the input file and the second armgument is the output file.
The input file should be MSP430 assembly. The output file is always llvm bitcode.
