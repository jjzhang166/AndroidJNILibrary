LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog -landroid_runtime -ldvm
#LOCAL_SRC_FILES += libmynet.cpp
LOCAL_SRC_FILES += localSocket.c
LOCAL_SRC_FILES += init2.cpp
LOCAL_SRC_FILES += JniUtil.cpp
LOCAL_SRC_FILES += JavaHook.cpp
LOCAL_SRC_FILES += JavaDump.cpp


LOCAL_MODULE := la
LOCAL_MODULE_TAGS := optional 

#LOCAL_ARM_MODE := arm
 
include $(BUILD_SHARED_LIBRARY)


