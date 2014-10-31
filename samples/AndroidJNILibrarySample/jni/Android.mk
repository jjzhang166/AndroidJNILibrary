AndroidJNILibrary_PATH = ../..
JNIJava_PATH = $(AndroidJNILibrary_PATH)/JNIJava

ROOT_LOCAL_PATH := $(call my-dir)

#---------------------------------------------------
# JNIJava
include $(JNIJava_PATH)/Android.mk

#---------------------------------------------------

LOCAL_PATH := $(ROOT_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := AndroidJNILibrarySample
#VisualGDBAndroid: AutoUpdateSourcesInNextLine
LOCAL_SRC_FILES := AndroidJNILibrarySample.cpp

LOCAL_C_INCLUDES := $(AndroidJNILibrary_PATH)

include $(BUILD_SHARED_LIBRARY)
