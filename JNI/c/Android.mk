LOCAL_PATH := $(call my-dir)

# libNetwork
include $(CLEAR_VARS)

LOCAL_MODULE := LIBARNETWORK-prebuilt
LOCAL_SRC_FILES := lib/libarnetwork.a

include $(PREBUILT_STATIC_LIBRARY)

# WRAPPER_LIB
include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include $(LOCAL_PATH)/../libARSAL/include $(LOCAL_PATH)/../libARNetworkAL/include
LOCAL_LDLIBS := -llog
LOCAL_MODULE := libarnetwork_android
LOCAL_SRC_FILES := ARNETWORK_JNIManager.c  ARNETWORK_JNIIOBufferParam.c
LOCAL_CFLAGS := -O0 -g
LOCAL_STATIC_LIBRARIES := LIBARNETWORK-prebuilt
LOCAL_SHARED_LIBRARIES := LIBARSAL-prebuilt LIBARNETWORKAL-prebuilt
include $(BUILD_SHARED_LIBRARY)

