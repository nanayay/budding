TOP_PATH := $(call my-dir)

##############################
# For libpng
include $(CLEAR_VARS)

include $(TOP_PATH)/external/libpng/jni/Android.mk

LOCAL_PATH := $(TOP_PATH)

##############################
# For OGLESSample_NDK
include $(CLEAR_VARS)

LS_CPP = $(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE    := OGLESSample_NDK
LOCAL_SRC_FILES :=  $(call LS_CPP,$(LOCAL_PATH))

LOCAL_LDLIBS    := -llog -landroid -lEGL -lz

#LOCAL_CFLAGS    += -DUSE_PNG_IN_SDCARD
#LOCAL_CFLAGS    += -DGLDEBUG
#LOCAL_CFLAGS    += -D_GDB_DEBUG_

GLESVersion = 2

ifeq ($(GLESVersion), 3)
    LOCAL_CFLAGS    += -DUSE_GLES3
    LOCAL_LDLIBS    += -lGLESv3
else ifeq ($(GLESVersion), 2)
    LOCAL_CFLAGS    += -DUSE_GLES2
    LOCAL_LDLIBS    += -lGLESv2
else ifeq ($(GLESVersion), 1)
    LOCAL_CFLAGS    += -DUSE_GLES1
    LOCAL_LDLIBS    += -lGLESv1_CM
else
    LOCAL_CFLAGS    += -DUSE_GLES2
    LOCAL_LDLIBS    += -lGLESv2
endif

LOCAL_STATIC_LIBRARIES	:= android_native_app_glue libpng

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
