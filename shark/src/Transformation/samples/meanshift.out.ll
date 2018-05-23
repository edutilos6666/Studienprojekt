; ModuleID = 'meanshift.out.bc'
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
  br label %9, !instruction !5

; <label>:9:                                      ; preds = %2
  %10 = load double, double* %8, align 8, !instruction !4
  %11 = fsub double %7, %10, !instruction !4
  store double %11, double* %3, align 8, !instruction !4
  %12 = getelementptr inbounds %struct.vec3, %struct.vec3* %0, i32 0, i32 1, !instruction !4
  %13 = load double, double* %12, align 8, !instruction !4
  %14 = getelementptr inbounds %struct.vec3, %struct.vec3* %1, i32 0, i32 1, !instruction !4
  br label %15, !instruction !5

; <label>:15:                                     ; preds = %9
  %16 = load double, double* %14, align 8, !instruction !4
  %17 = fsub double %13, %16, !instruction !4
  store double %17, double* %4, align 8, !instruction !4
  %18 = getelementptr inbounds %struct.vec3, %struct.vec3* %0, i32 0, i32 2, !instruction !4
  %19 = load double, double* %18, align 8, !instruction !4
  %20 = getelementptr inbounds %struct.vec3, %struct.vec3* %1, i32 0, i32 2, !instruction !4
  br label %21, !instruction !5

; <label>:21:                                     ; preds = %15
  %22 = load double, double* %20, align 8, !instruction !4
  %23 = fsub double %19, %22, !instruction !4
  store double %23, double* %5, align 8, !instruction !4
  %24 = load double, double* %3, align 8, !instruction !4
  %25 = load double, double* %3, align 8, !instruction !4
  %26 = fmul double %24, %25, !instruction !4
  br label %27, !instruction !5

; <label>:27:                                     ; preds = %21
  %28 = load double, double* %4, align 8, !instruction !4
  %29 = load double, double* %4, align 8, !instruction !4
  %30 = fmul double %28, %29, !instruction !4
  %31 = fadd double %26, %30, !instruction !4
  %32 = load double, double* %5, align 8, !instruction !4
  %33 = load double, double* %5, align 8, !instruction !4
  br label %34, !instruction !5

; <label>:34:                                     ; preds = %27
  %NOP = add i1 false, false, !instruction !6
  %35 = fmul double %32, %33, !instruction !4
  %36 = fadd double %31, %35, !instruction !4
  ret double %36, !instruction !4
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @epanechnikovKernel(double) #0 {
  %NOP = add i1 false, false, !instruction !7
  %2 = alloca double, align 8, !instruction !4
  %3 = alloca double, align 8, !instruction !4
  store double %0, double* %3, align 8, !instruction !4
  %4 = load double, double* %3, align 8, !instruction !4
  %5 = fcmp olt double %4, 1.000000e+00, !instruction !4
  br i1 %5, label %6, label %9, !instruction !4

; <label>:6:                                      ; preds = %1
  %NOP1 = add i1 false, false, !instruction !8
  %7 = load double, double* %3, align 8, !instruction !4
  %8 = fsub double 1.000000e+00, %7, !instruction !4
  store double %8, double* %2, align 8, !instruction !4
  br label %10, !instruction !4

; <label>:9:                                      ; preds = %1
  %NOP2 = add i1 false, false, !instruction !9
  store double 0.000000e+00, double* %2, align 8, !instruction !4
  br label %10, !instruction !4

; <label>:10:                                     ; preds = %9, %6
  %NOP3 = add i1 false, false, !instruction !9
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
  br label %10, !instruction !5

; <label>:10:                                     ; preds = %3
  %NOP = add i1 false, false, !instruction !8
  store %struct.vec3* %1, %struct.vec3** %4, align 8, !instruction !4
  store %struct.vec3* %2, %struct.vec3** %5, align 8, !instruction !4
  store i32 0, i32* %6, align 4, !instruction !4
  br label %11, !instruction !4

; <label>:11:                                     ; preds = %54, %10
  %NOP1 = add i1 false, false, !instruction !6
  %12 = load i32, i32* %6, align 4, !instruction !4
  %13 = icmp slt i32 %12, 40, !instruction !4
  br i1 %13, label %14, label %57, !instruction !4

; <label>:14:                                     ; preds = %11
  %15 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %16 = load i32, i32* %6, align 4, !instruction !4
  %17 = sext i32 %16 to i64, !instruction !4
  %18 = getelementptr inbounds %struct.vec3, %struct.vec3* %15, i64 %17, !instruction !4
  %19 = bitcast %struct.vec3* %7 to i8*, !instruction !4
  %20 = bitcast %struct.vec3* %18 to i8*, !instruction !4
  br label %21, !instruction !5

; <label>:21:                                     ; preds = %14
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %19, i8* %20, i64 24, i32 8, i1 false), !instruction !4
  %22 = call double @L2(%struct.vec3* byval align 8 %0, %struct.vec3* byval align 8 %7), !instruction !4
  store double %22, double* %8, align 8, !instruction !4
  %23 = load double, double* %8, align 8, !instruction !4
  %24 = call double @epanechnikovKernel(double %23), !instruction !4
  store double %24, double* %9, align 8, !instruction !4
  br label %25, !instruction !5

; <label>:25:                                     ; preds = %21
  %26 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 0, !instruction !4
  %27 = load double, double* %26, align 8, !instruction !4
  %28 = load double, double* %9, align 8, !instruction !4
  %29 = fmul double %27, %28, !instruction !4
  %30 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %31 = getelementptr inbounds %struct.vec3, %struct.vec3* %30, i32 0, i32 0, !instruction !4
  br label %32, !instruction !5

; <label>:32:                                     ; preds = %25
  %33 = load double, double* %31, align 8, !instruction !4
  %34 = fadd double %33, %29, !instruction !4
  store double %34, double* %31, align 8, !instruction !4
  %35 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 1, !instruction !4
  %36 = load double, double* %35, align 8, !instruction !4
  %37 = load double, double* %9, align 8, !instruction !4
  br label %38, !instruction !5

; <label>:38:                                     ; preds = %32
  %39 = fmul double %36, %37, !instruction !4
  %40 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %41 = getelementptr inbounds %struct.vec3, %struct.vec3* %40, i32 0, i32 1, !instruction !4
  %42 = load double, double* %41, align 8, !instruction !4
  %43 = fadd double %42, %39, !instruction !4
  store double %43, double* %41, align 8, !instruction !4
  br label %44, !instruction !5

; <label>:44:                                     ; preds = %38
  %45 = getelementptr inbounds %struct.vec3, %struct.vec3* %7, i32 0, i32 2, !instruction !4
  %46 = load double, double* %45, align 8, !instruction !4
  %47 = load double, double* %9, align 8, !instruction !4
  %48 = fmul double %46, %47, !instruction !4
  %49 = load %struct.vec3*, %struct.vec3** %5, align 8, !instruction !4
  %50 = getelementptr inbounds %struct.vec3, %struct.vec3* %49, i32 0, i32 2, !instruction !4
  br label %51, !instruction !5

; <label>:51:                                     ; preds = %44
  %NOP2 = add i1 false, false, !instruction !8
  %52 = load double, double* %50, align 8, !instruction !4
  %53 = fadd double %52, %48, !instruction !4
  store double %53, double* %50, align 8, !instruction !4
  br label %54, !instruction !4

; <label>:54:                                     ; preds = %51
  %NOP3 = add i1 false, false, !instruction !8
  %55 = load i32, i32* %6, align 4, !instruction !4
  %56 = add nsw i32 %55, 1, !instruction !4
  store i32 %56, i32* %6, align 4, !instruction !4
  br label %11, !instruction !4

; <label>:57:                                     ; preds = %11
  %NOP4 = add i1 false, false, !instruction !10
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
  br label %9, !instruction !5

; <label>:9:                                      ; preds = %2
  %10 = alloca %struct.vec3, align 8, !instruction !4
  %11 = alloca i32, align 4, !instruction !4
  %12 = alloca double, align 8, !instruction !4
  store %struct.vec3* %0, %struct.vec3** %3, align 8, !instruction !4
  store %struct.vec3* %1, %struct.vec3** %4, align 8, !instruction !4
  store double 2.500000e-09, double* %5, align 8, !instruction !4
  br label %13, !instruction !5

; <label>:13:                                     ; preds = %9
  %NOP = add i1 false, false, !instruction !9
  store i32 0, i32* %7, align 4, !instruction !4
  br label %14, !instruction !4

; <label>:14:                                     ; preds = %21, %13
  %NOP1 = add i1 false, false, !instruction !6
  %15 = load i32, i32* %7, align 4, !instruction !4
  %16 = icmp slt i32 %15, 40, !instruction !4
  br i1 %16, label %17, label %24, !instruction !4

; <label>:17:                                     ; preds = %14
  %NOP2 = add i1 false, false, !instruction !11
  %18 = load i32, i32* %7, align 4, !instruction !4
  %19 = sext i32 %18 to i64, !instruction !4
  %20 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %19, !instruction !4
  store i32 0, i32* %20, align 4, !instruction !4
  br label %21, !instruction !4

; <label>:21:                                     ; preds = %17
  %NOP3 = add i1 false, false, !instruction !8
  %22 = load i32, i32* %7, align 4, !instruction !4
  %23 = add nsw i32 %22, 1, !instruction !4
  store i32 %23, i32* %7, align 4, !instruction !4
  br label %14, !instruction !4

; <label>:24:                                     ; preds = %14
  %NOP4 = add i1 false, false, !instruction !10
  br label %25, !instruction !4

; <label>:25:                                     ; preds = %73, %24
  %NOP5 = add i1 false, false, !instruction !6
  store double 0.000000e+00, double* %8, align 8, !instruction !4
  store i32 0, i32* %11, align 4, !instruction !4
  br label %26, !instruction !4

; <label>:26:                                     ; preds = %69, %25
  %NOP6 = add i1 false, false, !instruction !6
  %27 = load i32, i32* %11, align 4, !instruction !4
  %28 = icmp slt i32 %27, 40, !instruction !4
  br i1 %28, label %29, label %72, !instruction !4

; <label>:29:                                     ; preds = %26
  %NOP7 = add i1 false, false, !instruction !7
  %30 = load i32, i32* %11, align 4, !instruction !4
  %31 = sext i32 %30 to i64, !instruction !4
  %32 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %31, !instruction !4
  %33 = load i32, i32* %32, align 4, !instruction !4
  %34 = icmp ne i32 %33, 0, !instruction !4
  br i1 %34, label %68, label %35, !instruction !4

; <label>:35:                                     ; preds = %29
  %36 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %37 = load i32, i32* %11, align 4, !instruction !4
  %38 = sext i32 %37 to i64, !instruction !4
  %39 = getelementptr inbounds %struct.vec3, %struct.vec3* %36, i64 %38, !instruction !4
  %40 = load %struct.vec3*, %struct.vec3** %3, align 8, !instruction !4
  call void @shift_point(%struct.vec3* byval align 8 %39, %struct.vec3* %40, %struct.vec3* %10), !instruction !4
  br label %41, !instruction !5

; <label>:41:                                     ; preds = %35
  %42 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %43 = load i32, i32* %11, align 4, !instruction !4
  %44 = sext i32 %43 to i64, !instruction !4
  %45 = getelementptr inbounds %struct.vec3, %struct.vec3* %42, i64 %44, !instruction !4
  %46 = call double @L2(%struct.vec3* byval align 8 %10, %struct.vec3* byval align 8 %45), !instruction !4
  store double %46, double* %12, align 8, !instruction !4
  br label %47, !instruction !5

; <label>:47:                                     ; preds = %41
  %NOP8 = add i1 false, false, !instruction !8
  %48 = load double, double* %12, align 8, !instruction !4
  %49 = load double, double* %8, align 8, !instruction !4
  %50 = fcmp ogt double %48, %49, !instruction !4
  br i1 %50, label %51, label %53, !instruction !4

; <label>:51:                                     ; preds = %47
  %NOP9 = add i1 false, false, !instruction !6
  %52 = load double, double* %12, align 8, !instruction !4
  store double %52, double* %8, align 8, !instruction !4
  br label %53, !instruction !4

; <label>:53:                                     ; preds = %51, %47
  %NOP10 = add i1 false, false, !instruction !6
  %54 = load double, double* %12, align 8, !instruction !4
  %55 = fcmp ole double %54, 2.500000e-09, !instruction !4
  br i1 %55, label %56, label %60, !instruction !4

; <label>:56:                                     ; preds = %53
  %NOP11 = add i1 false, false, !instruction !11
  %57 = load i32, i32* %11, align 4, !instruction !4
  %58 = sext i32 %57 to i64, !instruction !4
  %59 = getelementptr inbounds [40 x i32], [40 x i32]* %6, i64 0, i64 %58, !instruction !4
  store i32 1, i32* %59, align 4, !instruction !4
  br label %60, !instruction !4

; <label>:60:                                     ; preds = %56, %53
  %61 = load %struct.vec3*, %struct.vec3** %4, align 8, !instruction !4
  %62 = load i32, i32* %11, align 4, !instruction !4
  %63 = sext i32 %62 to i64, !instruction !4
  %64 = getelementptr inbounds %struct.vec3, %struct.vec3* %61, i64 %63, !instruction !4
  %65 = bitcast %struct.vec3* %64 to i8*, !instruction !4
  %66 = bitcast %struct.vec3* %10 to i8*, !instruction !4
  br label %67, !instruction !5

; <label>:67:                                     ; preds = %60
  %NOP12 = add i1 false, false, !instruction !9
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %65, i8* %66, i64 24, i32 8, i1 false), !instruction !4
  br label %68, !instruction !4

; <label>:68:                                     ; preds = %67, %29
  %NOP13 = add i1 false, false, !instruction !10
  br label %69, !instruction !4

; <label>:69:                                     ; preds = %68
  %NOP14 = add i1 false, false, !instruction !8
  %70 = load i32, i32* %11, align 4, !instruction !4
  %71 = add nsw i32 %70, 1, !instruction !4
  store i32 %71, i32* %11, align 4, !instruction !4
  br label %26, !instruction !4

; <label>:72:                                     ; preds = %26
  %NOP15 = add i1 false, false, !instruction !10
  br label %73, !instruction !4

; <label>:73:                                     ; preds = %72
  %NOP16 = add i1 false, false, !instruction !6
  %74 = load double, double* %8, align 8, !instruction !4
  %75 = fcmp ogt double %74, 2.500000e-09, !instruction !4
  br i1 %75, label %25, label %76, !instruction !4

; <label>:76:                                     ; preds = %73
  %NOP17 = add i1 false, false, !instruction !10
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
!5 = !{!"br", i64 1}
!6 = !{!"NOP", i64 7}
!7 = !{!"NOP", i64 1}
!8 = !{!"NOP", i64 5}
!9 = !{!"NOP", i64 9}
!10 = !{!"NOP", i64 11}
!11 = !{!"NOP", i64 3}
