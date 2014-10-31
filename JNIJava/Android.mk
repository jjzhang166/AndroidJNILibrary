LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := JNIJava
LOCAL_C_INCLUDES := ../utils

LOCAL_SRC_FILES := ./codes/JavaBase.cpp \
				   ./codes/JavaNumber.cpp \
				   ./codes/JavaString.cpp \
				   ./codes/JavaIO.cpp \
				   ./codes/JNIReflect.cpp \
				   ./codes/AndroidUtil.cpp \
				   ./codes/AndroidContent.cpp
				   

include $(BUILD_STATIC_LIBRARY)
