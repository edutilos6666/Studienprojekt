llvm-as newton.out.ll -o newton.out.bc does 
not produce .bc file with the following error message: <br/>
"
llvm-as: assembly parsed, but does not verify as correct!
PHI nodes not grouped at top of basic block!
  %33 = phi i1 [ false, %26 ], [ %31, %29 ], !instruction !4
label %32
"