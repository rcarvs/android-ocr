###                                               _______ _______  _______
 #   _ __  ___ _____   ___   __  __   ___ __     /  _   //  ____/ /  _   /
 #  |  _ \/ _ |  _  | / _ | / / / /  / __/ /    /  //  //  /     /  |_| /
 #  |  __/ __ |  ___|/ __ |/ /_/ /__/ __/ /__  /  //  //  /____ /  ____/
 #  |_| /_/ |_|_|\_\/_/ |_/____/___/___/____/ /______//_______//__|\__\
 #
 ##

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := ParallelUSRuntime
LOCAL_MODULE := ParallelOCR
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Ofast -Wall -Wextra -Werror
LOCAL_CPPFLAGS := -Ofast -Wall -Wextra -Werror -std=c++14 -fexceptions
LOCAL_CPP_FEATURES += exceptions
LOCAL_LDLIBS := -llog -ldl -ljnigraphics -landroid
LOCAL_SRC_FILES := src/parallelocr/Letter.cpp \
		           src/parallelocr/Coach.cpp \
                   src/parallelocr/Image.cpp 
include $(BUILD_SHARED_LIBRARY)
