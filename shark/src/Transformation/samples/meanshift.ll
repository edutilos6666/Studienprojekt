; ModuleID = './meanshift.c'
source_filename = "./meanshift.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.vec3 = type { double, double, double }

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @L2(%struct.vec3* byval align 8, %struct.vec3* byval align 8) #0 {
  %3 = alloca double, align 8, !instruction !4
  %4 = alloca double, align 8, !instruction !4
  %5 = alloca double, align 8, !instruction !4
  %6 = getelementptr inbounds %struct.vec3, %struct.vec3* %0, i32 0, i32 0, !instruction !4
  %7 = load double, double* %6, align 8, !instruction !4
  %8 = getelementptr inbounds %struct.vec3, %struct.vec3* %1, i32 0, i32 0, !instruction !4
  %9 = load double, double* %8, align 8, !instruction !4
  %10 = fsub double %7, %9, !instruction !4
  store double %10, double* %3, align 8, !instruction !4
  %11 = getelementptr inbounds %struct.vec3, %struct.vec3* %0, i32 0, i32 1, !instruction !4
  %12 = load double, double* %11, align 8, !instruction !4
  %13 = getelementptr inbounds %struct.vec3, %struct.vec3* %1, i32 0, i32 1, !instruction !4
  %14 = load double, double* %13, align 8, !instruction !4
  %15 = fsub double %12, %14, !instruction !4
  store double %15, double* %4, align 8, !instruction !4
  %16 = getelementptr inbounds %struct.vec3, %struct.vec3* %0, i32 0, i32 2, !instruction !4
  %17 = load double, double* %16, align 8, !instruction !4
  %18 = getelementptr inbounds %struct.vec3, %struct.vec3* %1, i32 0, i32 2, !instruction !4
  %19 = load double, double* %18, align 8, !instruction !4
  %20 = fsub double %17, %19, !instruction !4
  store double %20, double* %5, align 8, !instruction !4
  %21 = load double, double* %3, align 8, !instruction !4
  %22 = load double, double* %3, align 8, !instruction !4
  %23 = fmul double %21, %22, !instruction !4
  %24 = load double, double* %4, align 8, !instruction !4
  %25 = load double, double* %4, align 8, !instruction !4
  %26 = fmul double %24, %25, !instruction !4
  %27 = fadd double %23, %26, !instruction !4
  %28 = load double, double* %5, align 8, !instruction !4
  %29 = load double, double* %5, align 8, !instruction !4
  %30 = fmul double %28, %29, !instruction !4
  %31 = fadd double %27, %30, !instruction !4
  ret double %31, !instruction !4
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @epanechnikovKernel(double) #0 {
  %2 = alloca double, align 8, !instruction !4
  %3 = alloca double, align 8, !instruction !4
  store double %0, double* %3, align 8, !instruction !4
  %4 = load double, double* %3, align 8, !instruction !4
  %5 = fcmp olt double %4, 1.000000e+00, !instruction !4
  br i1 %5, label %6, label %9, !instruction !4

; <label>:6:                                      ; preds = %1
  %7 = load double, double* %3, align 8, !instruction !4
  %8 = fsub double 1.000000e+00, %7, !instruction !4
  store double %8, double* %2, align 8, !instruction !4
  br label %10, !instruction !4

; <label>:9:                                      ; preds = %1
  store double 0.000000e+00, double* %2, align 8, !instruction !4
  br label %10, !instruction !4

; <label>:10:                                     ; preds = %9, %6
  %11 = load double, double* %2, align 8, !instruction !4
  ret double %11, !instruction !4
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define void @shift_point(%struct.vec3* byval align 8, %struct.vec3*, %struct.vec3*) #0 {
  %4 = alloca %struct.vec3*, align 8, !instruction !4
  %5 = alloca %struct.vec3*, align 8, !instruction !4
  %6 = alloca i32, align 4, !instruction !4
  %7 = alloca %struct.vec3, align 8, !instruction !4
  %8 = alloca double, align 8, !instruction !4
  %9 = alloca double, align 8, !instruction !4
  store %struct.vec3* %1, %struct.vec3** %4, align 8, !instruction !4
  store %struct.vec3* %2, %struct.vec3** %5, align 8, !instruction !4
  store i32 0, i32* %6, align 4, !instruction !4
  br label %10, !instruction !4

; <label>:10:                                     ; preds = %47, %3
  %11 = load i32, i32* %6, align 4, !instruction !4
  %12 = icmp slt i32 %11, 40, !instruction !4
  br i1 %12, label %13, label %50, !instruction !4

; <label>:13:                                     ; preds = %10
  %14 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %15 = load i32, i32* %6, align 4, !instruction !4
  %16 = sext i32 %15 to i64, !instruction !4
  %17 = getelementptr inbounds %struct.vec3, %struct.vec3* %14, i64 %16, !instruction !4
  %18 = bitcast %struct.vec3* %7 to i8*, !instruction !4
  %19 = bitcast %struct.vec3* %17 to i8*, !instruction !4
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %18, i8* %19, i64 24, i32 8, i1 false), !instruction !4
  %20 = call double @L2(%struct.vec3* byval align 8 %0, %struct.vec3* byval align 8 %7), !instruction !4
  store double %20, double* %8, align 8, !instruction !4
  %21 = load double, double* %8, align 8, !instruction !4
  %22 = call double @epanechnikovKernel(double %21), !instruction !4
  store double %22, double* %9, align 8, !instruction !4
  %23 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 0, !instruction !4
  %24 = load double, double* %23, align 8, !instruction !4
  %25 = load double, double* %9, align 8, !instruction !4
  %26 = fmul double %24, %25, !instruction !4
  %27 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %28 = getelementptr inbounds %struct.vec3, %struct.vec3* %27, i32 0, i32 0, !instruction !4
  %29 = load double, double* %28, align 8, !instruction !4
  %30 = fadd double %29, %26, !instruction !4
  store double %30, double* %28, align 8, !instruction !4
  %31 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 1, !instruction !4
  %32 = load double, double* %31, align 8, !instruction !4
  %33 = load double, double* %9, align 8, !instruction !4
  %34 = fmul double %32, %33, !instruction !4
  %35 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %36 = getelementptr inbounds %struct.vec3, %struct.vec3* %35, i32 0, i32 1, !instruction !4
  %37 = load double, double* %36, align 8, !instruction !4
  %38 = fadd double %37, %34, !instruction !4
  store double %38, double* %36, align 8, !instruction !4
  %39 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 2, !instruction !4
  %40 = load double, double* %39, align 8, !instruction !4
  %41 = load double, double* %9, align 8, !instruction !4
  %42 = fmul double %40, %41, !instruction !4
  %43 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %44 = getelementptr inbounds %struct.vec3, %struct.vec3* %43, i32 0, i32 2, !instruction !4
  %45 = load double, double* %44, align 8, !instruction !4
  %46 = fadd double %45, %42, !instruction !4
  store double %46, double* %44, align 8, !instruction !4
  br label %47, !instruction !4

; <label>:47:                                     ; preds = %13
  %48 = load i32, i32* %6, align 4, !instruction !4
  %49 = add nsw i32 %48, 1, !instruction !4
  store i32 %49, i32* %6, align 4, !instruction !4
  br label %10, !instruction !4

; <label>:50:                                     ; preds = %10
  ret void, !instruction !4
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define void @meanshift(%struct.vec3*, %struct.vec3*) #0 {
  %3 = alloca %struct.vec3*, align 8, !instruction !4
  %4 = alloca %struct.vec3*, align 8, !instruction !4
  %5 = alloca double, align 8, !instruction !4
  %6 = alloca [40 x i32], align 16, !instruction !4
  %7 = alloca i32, align 4, !instruction !4
  %8 = alloca double, align 8, !instruction !4
  %9 = alloca %struct.vec3, align 8, !instruction !4
  %10 = alloca i32, align 4, !instruction !4
  %11 = alloca double, align 8, !instruction !4
  store %struct.vec3* %0, %struct.vec3** %3, align 8, !instruction !4
  store %struct.vec3* %1, %struct.vec3** %4, align 8, !instruction !4
  store double 2.500000e-09, double* %5, align 8, !instruction !4
  store i32 0, i32* %7, align 4, !instruction !4
  br label %12, !instruction !4

; <label>:12:                                     ; preds = %19, %2
  %13 = load i32, i32* %7, align 4, !instruction !4
  %14 = icmp slt i32 %13, 40, !instruction !4
  br i1 %14, label %15, label %22, !instruction !4

; <label>:15:                                     ; preds = %12
  %16 = load i32, i32* %7, align 4, !instruction !4
  %17 = sext i32 %16 to i64, !instruction !4
  %18 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %17, !instruction !4
  store i32 0, i32* %18, align 4, !instruction !4
  br label %19, !instruction !4

; <label>:19:                                     ; preds = %15
  %20 = load i32, i32* %7, align 4, !instruction !4
  %21 = add nsw i32 %20, 1, !instruction !4
  store i32 %21, i32* %7, align 4, !instruction !4
  br label %12, !instruction !4

; <label>:22:                                     ; preds = %12
  br label %23, !instruction !4

; <label>:23:                                     ; preds = %68, %22
  store double 0.000000e+00, double* %8, align 8, !instruction !4
  store i32 0, i32* %10, align 4, !instruction !4
  br label %24, !instruction !4

; <label>:24:                                     ; preds = %64, %23
  %25 = load i32, i32* %10, align 4, !instruction !4
  %26 = icmp slt i32 %25, 40, !instruction !4
  br i1 %26, label %27, label %67, !instruction !4

; <label>:27:                                     ; preds = %24
  %28 = load i32, i32* %10, align 4, !instruction !4
  %29 = sext i32 %28 to i64, !instruction !4
  %30 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %29, !instruction !4
  %31 = load i32, i32* %30, align 4, !instruction !4
  %32 = icmp ne i32 %31, 0, !instruction !4
  br i1 %32, label %63, label %33, !instruction !4

; <label>:33:                                     ; preds = %27
  %34 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %35 = load i32, i32* %10, align 4, !instruction !4
  %36 = sext i32 %35 to i64, !instruction !4
  %37 = getelementptr inbounds %struct.vec3, %struct.vec3* %34, i64 %36, !instruction !4
  %38 = load %struct.vec3*, %struct.vec3** %3, align 8, !instruction !4
  call void @shift_point(%struct.vec3* byval align 8 %37, %struct.vec3* %38, %struct.vec3* %9), !instruction !4
  %39 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %40 = load i32, i32* %10, align 4, !instruction !4
  %41 = sext i32 %40 to i64, !instruction !4
  %42 = getelementptr inbounds %struct.vec3, %struct.vec3* %39, i64 %41, !instruction !4
  %43 = call double @L2(%struct.vec3* byval align 8 %9, %struct.vec3* byval align 8 %42), !instruction !4
  store double %43, double* %11, align 8, !instruction !4
  %44 = load double, double* %11, align 8, !instruction !4
  %45 = load double, double* %8, align 8, !instruction !4
  %46 = fcmp ogt double %44, %45, !instruction !4
  br i1 %46, label %47, label %49, !instruction !4

; <label>:47:                                     ; preds = %33
  %48 = load double, double* %11, align 8, !instruction !4
  store double %48, double* %8, align 8, !instruction !4
  br label %49, !instruction !4

; <label>:49:                                     ; preds = %47, %33
  %50 = load double, double* %11, align 8, !instruction !4
  %51 = fcmp ole double %50, 2.500000e-09, !instruction !4
  br i1 %51, label %52, label %56, !instruction !4

; <label>:52:                                     ; preds = %49
  %53 = load i32, i32* %10, align 4, !instruction !4
  %54 = sext i32 %53 to i64, !instruction !4
  %55 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %54, !instruction !4
  store i32 1, i32* %55, align 4, !instruction !4
  br label %56, !instruction !4

; <label>:56:                                     ; preds = %52, %49
  %57 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %58 = load i32, i32* %10, align 4, !instruction !4
  %59 = sext i32 %58 to i64, !instruction !4
  %60 = getelementptr inbounds %struct.vec3, %struct.vec3* %57, i64 %59, !instruction !4
  %61 = bitcast %struct.vec3* %60 to i8*, !instruction !4
  %62 = bitcast %struct.vec3* %9 to i8*, !instruction !4
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %61, i8* %62, i64 24, i32 8, i1 false), !instruction !4
  br label %63, !instruction !4

; <label>:63:                                     ; preds = %56, %27
  br label %64, !instruction !4

; <label>:64:                                     ; preds = %63
  %65 = load i32, i32* %10, align 4, !instruction !4
  %66 = add nsw i32 %65, 1, !instruction !4
  store i32 %66, i32* %10, align 4, !instruction !4
  br label %24, !instruction !4

; <label>:67:                                     ; preds = %24
  br label %68, !instruction !4

; <label>:68:                                     ; preds = %67
  %69 = load double, double* %8, align 8, !instruction !4
  %70 = fcmp ogt double %69, 2.500000e-09, !instruction !4
  br i1 %70, label %23, label %71, !instruction !4

; <label>:71:                                     ; preds = %68
  ret void, !instruction !4
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
!4 = !{!"mov.w #5, r6", i64 2}
