ROOT_LOCAL_PATH := $(call my-dir)
AndroidJNILibrary_PATH = $(ROOT_LOCAL_PATH)/../../..
JNIJava_PATH = $(AndroidJNILibrary_PATH)/JNIJava
SYSTEM_LIBRARIES = $(AndroidJNILibrary_PATH)/system_libraries

$(info commond=$(dir))

#---------------------------------------------------
# JNIJava
include $(JNIJava_PATH)/Android.mk

#---------------------------------------------------

LOCAL_PATH := $(ROOT_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := AndroidJNILibrarySample

LOCAL_C_INCLUDES := $(AndroidJNILibrary_PATH)

#VisualGDBAndroid: AutoUpdateSourcesInNextLine
LOCAL_SRC_FILES := AndroidJNILibrarySample.cpp

LOCAL_LDLIBS := -fuse-ld=bfd

LOCAL_STATIC_LIBRARIES := JNIJava
LOCAL_LDFLAGS += -L$(SYSTEM_LIBRARIES) -landroid_runtime_arm -ldvm_arm

include $(BUILD_SHARED_LIBRARY)
