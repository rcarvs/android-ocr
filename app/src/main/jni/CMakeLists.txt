cmake_minimum_required(VERSION 3.6)
project(jni)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    ParallelUS/runtime/include/parallelus/Buffer.hpp
    ParallelUS/runtime/include/parallelus/Device.hpp
    ParallelUS/runtime/include/parallelus/Kernel.hpp
    ParallelUS/runtime/include/parallelus/NetworkManager.hpp
    ParallelUS/runtime/include/parallelus/ParallelUS.hpp
    ParallelUS/runtime/include/parallelus/Program.hpp
    ParallelUS/runtime/include/parallelus/Runtime.hpp
    ParallelUS/runtime/include/parallelus/Scheduler.hpp
    ParallelUS/runtime/include/parallelus/SchedulerFCFS.hpp
    ParallelUS/runtime/include/parallelus/SchedulerHEFT.hpp
    ParallelUS/runtime/include/parallelus/SchedulerPAMS.hpp
    ParallelUS/runtime/include/parallelus/Task.hpp
    ParallelUS/runtime/src/parallelus/dynloader/headers/cl.h
    ParallelUS/runtime/src/parallelus/dynloader/headers/cl_platform.h
    ParallelUS/runtime/src/parallelus/dynloader/dynLoader.c
    ParallelUS/runtime/src/parallelus/dynloader/dynLoader.h
    ParallelUS/runtime/src/parallelus/util/error.h
    ParallelUS/runtime/src/parallelus/util/platform.h
    ParallelUS/runtime/src/parallelus/Buffer.cpp
    ParallelUS/runtime/src/parallelus/Device.cpp
    ParallelUS/runtime/src/parallelus/Kernel.cpp
    ParallelUS/runtime/src/parallelus/Network.hpp
    ParallelUS/runtime/src/parallelus/NetWorker.hpp
    ParallelUS/runtime/src/parallelus/NetworkManager.cpp
    ParallelUS/runtime/src/parallelus/Program.cpp
    ParallelUS/runtime/src/parallelus/Runtime.cpp
    ParallelUS/runtime/src/parallelus/SchedulerFCFS.cpp
    ParallelUS/runtime/src/parallelus/SchedulerHEFT.cpp
    ParallelUS/runtime/src/parallelus/SchedulerPAMS.cpp
    ParallelUS/runtime/src/parallelus/Task.cpp
    ParallelUS/runtime/src/parallelus/Worker.hpp
    ParallelOCR/include/parallelocr/Image.hpp
    ParallelOCR/include/parallelocr/Letter.hpp
    ParallelOCR/include/parallelocr/Coach.hpp
    ParallelOCR/include/parallelocr/ParallelOCR.hpp
    ParallelOCR/src/parallelocr/Image.cpp
    ParallelOCR/src/parallelocr/Letter.cpp
    ParallelOCR/src/parallelocr/Coach.cpp
    br_edu_ufsj_dcomp_ocr_Controller.h
    br_edu_ufsj_dcomp_ocr_Controller.cpp)

add_executable(jni ${SOURCE_FILES})