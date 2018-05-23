; ModuleID = './Transform/newton.c'
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
  %8 = alloca double, align 8, !instruction !4
  store double %0, double* %2, align 8, !instruction !4
  store double 5.000000e-03, double* %3, align 8, !instruction !4
  store i32 100, i32* %4, align 4, !instruction !4
  store double 1.000000e+00, double* %5, align 8, !instruction !4
  %9 = load double, double* %2, align 8, !instruction !4
  store double %9, double* %6, align 8, !instruction !4
  store i32 0, i32* %7, align 4, !instruction !4
  br label %10, !instruction !4

; <label>:10:                                     ; preds = %28, %1
  %11 = load double, double* %6, align 8, !instruction !4
  %12 = load double, double* %6, align 8, !instruction !4
  %13 = call double @f(double %12), !instruction !4
  %14 = load double, double* %6, align 8, !instruction !4
  %15 = call double @f_(double %14), !instruction !4
  %16 = fdiv double %13, %15, !instruction !4
  %17 = fsub double %11, %16, !instruction !4
  store double %17, double* %8, align 8, !instruction !4
  %18 = load double, double* %6, align 8, !instruction !4
  %19 = load double, double* %8, align 8, !instruction !4
  %20 = fsub double %18, %19, !instruction !4
  store double %20, double* %5, align 8, !instruction !4
  %21 = load double, double* %8, align 8, !instruction !4
  store double %21, double* %6, align 8, !instruction !4
  br label %22, !instruction !4

; <label>:22:                                     ; preds = %10
  %23 = load double, double* %5, align 8, !instruction !4
  %24 = fcmp ogt double %23, 5.000000e-03, !instruction !4
  br i1 %24, label %25, label %28, !instruction !4

; <label>:25:                                     ; preds = %22
  %26 = load i32, i32* %7, align 4, !instruction !4
  %27 = icmp slt i32 %26, 100, !instruction !4
  br label %28, !instruction !4

; <label>:28:                                     ; preds = %25, %22
  %29 = phi i1 [ false, %22 ], [ %27, %25 ], !instruction !4
  br i1 %29, label %10, label %30, !instruction !4

; <label>:30:                                     ; preds = %28
  %31 = load double, double* %6, align 8, !instruction !4
  ret double %31, !instruction !4
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
!4 = !{!"mov.w #5, r6", i64 2}
