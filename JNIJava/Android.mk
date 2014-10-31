LOCAL_PATH = $(call my-dir)

$(info LOCAL_PATH=$(LOCAL_PATH))

LOCAL_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := JNIJava
LOCAL_C_INCLUDES := ../utils

#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/codes/*.cpp)
#LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/codes/*.c)
LOCAL_SRC_FILES := $(LOCAL_PATH)/codes/JavaBase.cpp \
				   $(LOCAL_PATH)/codes/JavaNumber.cpp \
				   $(LOCAL_PATH)/codes/JavaString.cpp \
				   $(LOCAL_PATH)/codes/JavaIO.cpp \
				   $(LOCAL_PATH)/codes/JNIReflect.cpp \
				   $(LOCAL_PATH)/codes/AndroidUtil.cpp \
				   $(LOCAL_PATH)/codes/AndroidContent.cpp

#LOCAL_CFLAGS += -fvisibility=hidden

include $(BUILD_STATIC_LIBRARY)
