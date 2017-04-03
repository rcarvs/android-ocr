cmake_minimum_required(VERSION 3.6)
project(runtime)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    include/parallelme/Buffer.hpp
    include/parallelme/Device.hpp
    include/parallelme/Kernel.hpp
    include/parallelme/ParallelME.hpp
    include/parallelme/Program.hpp
    include/parallelme/Runtime.hpp
    include/parallelme/Scheduler.hpp
    include/parallelme/SchedulerFCFS.hpp
    include/parallelme/SchedulerHEFT.hpp
    include/parallelme/SchedulerPAMS.hpp
    include/parallelme/Task.hpp
    src/parallelme/dynloader/headers/cl.h
    src/parallelme/dynloader/headers/cl_platform.h
    src/parallelme/dynloader/dynLoader.c
    src/parallelme/dynloader/dynLoader.h
    src/parallelme/util/error.h
    src/parallelme/util/platform.h
    src/parallelme/Buffer.cpp
    src/parallelme/Device.cpp
    src/parallelme/Kernel.cpp
    src/parallelme/Program.cpp
    src/parallelme/Runtime.cpp
    src/parallelme/SchedulerFCFS.cpp
    src/parallelme/SchedulerHEFT.cpp
    src/parallelme/SchedulerPAMS.cpp
    src/parallelme/Task.cpp
    src/parallelme/Worker.hpp
    Android.mk
    Doxyfile
    LICENSE
    README.md)

add_executable(runtime ${SOURCE_FILES})