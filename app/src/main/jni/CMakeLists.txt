cmake_minimum_required(VERSION 3.6)
project(jni)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    ParallelME/runtime/include/parallelme/Buffer.hpp
    ParallelME/runtime/include/parallelme/Device.hpp
    ParallelME/runtime/include/parallelme/Kernel.hpp
    ParallelME/runtime/include/parallelme/ParallelME.hpp
    ParallelME/runtime/include/parallelme/Program.hpp
    ParallelME/runtime/include/parallelme/Runtime.hpp
    ParallelME/runtime/include/parallelme/Scheduler.hpp
    ParallelME/runtime/include/parallelme/SchedulerFCFS.hpp
    ParallelME/runtime/include/parallelme/SchedulerHEFT.hpp
    ParallelME/runtime/include/parallelme/SchedulerPAMS.hpp
    ParallelME/runtime/include/parallelme/Task.hpp
    ParallelME/runtime/src/parallelme/dynloader/headers/cl.h
    ParallelME/runtime/src/parallelme/dynloader/headers/cl_platform.h
    ParallelME/runtime/src/parallelme/dynloader/dynLoader.c
    ParallelME/runtime/src/parallelme/dynloader/dynLoader.h
    ParallelME/runtime/src/parallelme/util/error.h
    ParallelME/runtime/src/parallelme/util/platform.h
    ParallelME/runtime/src/parallelme/Buffer.cpp
    ParallelME/runtime/src/parallelme/Device.cpp
    ParallelME/runtime/src/parallelme/Kernel.cpp
    ParallelME/runtime/src/parallelme/Program.cpp
    ParallelME/runtime/src/parallelme/Runtime.cpp
    ParallelME/runtime/src/parallelme/SchedulerFCFS.cpp
    ParallelME/runtime/src/parallelme/SchedulerHEFT.cpp
    ParallelME/runtime/src/parallelme/SchedulerPAMS.cpp
    ParallelME/runtime/src/parallelme/Task.cpp
    ParallelME/runtime/src/parallelme/Worker.hpp
    ParallelOCR/include/parallelocr/Image.hpp
    ParallelOCR/include/parallelocr/RGB.hpp
    ParallelOCR/include/parallelocr/Letter.hpp
    ParallelOCR/include/parallelocr/ParallelOCR.hpp
    ParallelOCR/src/parallelocr/Image.cpp
    ParallelOCR/src/parallelocr/Letter.cpp
    br_edu_ufsj_dcomp_ocr_Controller.h
    br_edu_ufsj_dcomp_ocr_Controller.cpp)

add_executable(jni ${SOURCE_FILES})