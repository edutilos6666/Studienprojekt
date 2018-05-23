; ModuleID = 'newton.out.bc'
source_filename = "./Transform/newton.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @f(double) #0 {
  %2 = alloca double, align 8, !instruction !4
  store double %0, double* %2, align 8, !instruction !4
  %3 = load double, double* %2, align 8, !instruction !4
  %4 = load double, double* %2, align 8, !instruction !4
  %5 = fmul double %3, %4, !instruction !4
  ret double %5, !instruction !4
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @f_(double) #0 {
  %NOP = add i1 false, false, !instruction !5
  %2 = alloca double, align 8, !instruction !4
  store double %0, double* %2, align 8, !instruction !4
  %3 = load double, double* %2, align 8, !instruction !4
  %4 = fmul double 2.000000e+00, %3, !instruction !4
  ret double %4, !instruction !4
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @find_root(double) #0 {
  %2 = alloca double, align 8, !instruction !4
  %3 = alloca double, align 8, !instruction !4
  %4 = alloca i32, align 4, !instruction !4
  %5 = alloca double, align 8, !instruction !4
  %6 = alloca double, align 8, !instruction !4
  %7 = alloca i32, align 4, !instruction !4
  br label %8, !instruction !6

; <label>:8:                                      ; preds = %1
  %9 = alloca double, align 8, !instruction !4
  store double %0, double* %2, align 8, !instruction !4
  store double 5.000000e-03, double* %3, align 8, !instruction !4
  store i32 100, i32* %4, align 4, !instruction !4
  store double 1.000000e+00, double* %5, align 8, !instruction !4
  %10 = load double, double* %2, align 8, !instruction !4
  br label %11, !instruction !6

; <label>:11:                                     ; preds = %8
  %NOP = add i1 false, false, !instruction !7
  store double %10, double* %6, align 8, !instruction !4
  store i32 0, i32* %7, align 4, !instruction !4
  br label %12, !instruction !4

; <label>:12:                                     ; preds = %32, %11
  %13 = load double, double* %6, align 8, !instruction !4
  %14 = load double, double* %6, align 8, !instruction !4
  %15 = call double @f(double %14), !instruction !4
  %16 = load double, double* %6, align 8, !instruction !4
  %17 = call double @f_(double %16), !instruction !4
  %18 = fdiv double %15, %17, !instruction !4
  br label %19, !instruction !6

; <label>:19:                                     ; preds = %12
  %20 = fsub double %13, %18, !instruction !4
  store double %20, double* %9, align 8, !instruction !4
  %21 = load double, double* %6, align 8, !instruction !4
  %22 = load double, double* %9, align 8, !instruction !4
  %23 = fsub double %21, %22, !instruction !4
  store double %23, double* %5, align 8, !instruction !4
  br label %24, !instruction !6

; <label>:24:                                     ; preds = %19
  %NOP1 = add i1 false, false, !instruction !7
  %25 = load double, double* %9, align 8, !instruction !4
  store double %25, double* %6, align 8, !instruction !4
  br label %26, !instruction !4

; <label>:26:                                     ; preds = %24
  %NOP2 = add i1 false, false, !instruction !7
  %27 = load double, double* %5, align 8, !instruction !4
  %28 = fcmp ogt double %27, 5.000000e-03, !instruction !4
  br i1 %28, label %29, label %32, !instruction !4

; <label>:29:                                     ; preds = %26
  %NOP3 = add i1 false, false, !instruction !7
  %30 = load i32, i32* %7, align 4, !instruction !4
  %31 = icmp slt i32 %30, 100, !instruction !4
  br label %32, !instruction !4

; <label>:32:                                     ; preds = %29, %26
  %NOP4 = add i1 false, false, !instruction !8
  %33 = phi i1 [ false, %26 ], [ %31, %29 ], !instruction !4
  br i1 %33, label %12, label %34, !instruction !4

; <label>:34:                                     ; preds = %32
  %NOP5 = add i1 false, false, !instruction !8
  %35 = load double, double* %6, align 8, !instruction !4
  ret double %35, !instruction !4
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
!4 = !{!"mov.w #5, r6", i64 2}
!5 = !{!"NOP", i64 2}
!6 = !{!"br", i64 1}
!7 = !{!"NOP", i64 6}
!8 = !{!"NOP", i64 8}
