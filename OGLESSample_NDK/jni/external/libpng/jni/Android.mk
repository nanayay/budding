LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += 

LS_C = $(subst $(1)/,,$(wildcard $(1)/*.c))

LOCAL_MODULE    := libpng
LOCAL_SRC_FILES := \
    $(filter-out example.c pngtest.c,$(call LS_C,$(LOCAL_PATH)))
    
LOCAL_LDLIBS := -lz

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)

#For shared library:
#include $(BUILD_SHARED_LIBRARY)
