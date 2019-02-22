LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog
LOCAL_MODULE    := testprocess
LOCAL_SRC_FILES := testprocess.cpp



include $(BUILD_SHARED_LIBRARY)