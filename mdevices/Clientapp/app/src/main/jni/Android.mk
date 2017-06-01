	###                                               _    __ ____
 #   _ __  ___ _____   ___   __  __   ___ __     / |  / /  __/
 #  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  | / / /__
 #  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  / / v  / /__
 #  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /_/  /_/____/
 #
 ##

PM_JNI_PATH := $(call my-dir)/ParallelUS
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := Cliente
LOCAL_C_INCLUDES := $(PM_JNI_PATH)/runtime/include
LOCAL_LDLIBS := -llog
LOCAL_LDFLAGS += -ljnigraphics
LOCAL_CPPFLAGS := -Ofast -Wall -Wextra -Werror -Wno-deprecated-register -Wunused-function -Wno-unused-parameter -std=c++14 -llog -stdlib=libc++
LOCAL_SHARED_LIBRARIES := ParallelUSRuntime
LOCAL_SRC_FILES := br_edu_ufsj_dcomp_clientapp_Cliente.cpp

include $(BUILD_SHARED_LIBRARY)
include $(wildcard $(PM_JNI_PATH)/**/Android.mk)
