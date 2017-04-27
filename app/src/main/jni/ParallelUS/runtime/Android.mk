###                                               _    __ ____
 #   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 #  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 #  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 #  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 #
 ##

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := ParallelUSRuntime
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Ofast -Wall -Wextra -Werror 
LOCAL_CPPFLAGS := -Ofast -Wall -Wextra -Wno-deprecated-register -Werror -std=c++14 -fexceptions
LOCAL_CPP_FEATURES += exceptions
LOCAL_LDLIBS := -llog -ldl -ljnigraphics
LOCAL_SRC_FILES := src/parallelus/Buffer.cpp src/parallelus/Device.cpp \
	src/parallelus/Kernel.cpp src/parallelus/Program.cpp \
	src/parallelus/Runtime.cpp src/parallelus/Task.cpp src/parallelus/NetworkManager.cpp \
	src/parallelus/SchedulerFCFS.cpp src/parallelus/SchedulerHEFT.cpp \
	src/parallelus/SchedulerPAMS.cpp src/parallelus/dynloader/dynLoader.c
include $(BUILD_SHARED_LIBRARY)
