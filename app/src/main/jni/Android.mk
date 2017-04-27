
LOCAL_PATH := $(call my-dir)
PM_JNI_PATH := $(call my-dir)/ParallelME
PU_JNI_PATH := $(call my-dir)/ParallelUS
OCR_JNI_PATH := $(call my-dir)/ParallelOCR


include $(CLEAR_VARS)
LOCAL_MODULE := OCR
LOCAL_C_INCLUDES := $(PU_JNI_PATH)/runtime/include $(OCR_JNI_PATH)/include

LOCAL_CPPFLAGS := -Ofast -Wall -Wextra -Werror -Wno-unused-parameter -std=c++14
LOCAL_SHARED_LIBRARIES := ParallelUSRuntime ParallelOCR

LOCAL_SRC_FILES := br_edu_ufsj_dcomp_ocr_Controller.cpp

LOCAL_LDLIBS := -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)

include $(wildcard $(PU_JNI_PATH)/**/Android.mk)
include $(OCR_JNI_PATH)/Android.mk