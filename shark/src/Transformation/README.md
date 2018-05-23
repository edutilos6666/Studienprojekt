## Usage
Just call the program from the command line with the input file as the first argument and the output file as the second one. The input file can be human readable llvm assembly (.ll) or llvm bitcode (.bc) form and must be annotated. The output file is always llvm bitcode (.bc).


## How to construct consumable input files?
1. Use the frontend
2. Compile a file to llvm bc, disassemble and annotate instructions manually, according to metadata specification
