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

ifeq ($(USE_GLES3), true)
    LOCAL_CFLAGS    += -DUSE_GLES3
    LOCAL_LDLIBS    += -lGLESv3
else ifeq ($(USE_GLES2), true)
    LOCAL_CFLAGS    += -DUSE_GLES2
    LOCAL_LDLIBS    += -lGLESv2
else ifeq ($(USE_GLES1), true)
    LOCAL_CFLAGS    += -DUSE_GLES1
    LOCAL_LDLIBS    += -lGLESv1_CM
else
    LOCAL_CFLAGS    += -DUSE_GLES3
    LOCAL_LDLIBS    += -lGLESv3
endif

LOCAL_STATIC_LIBRARIES	:= android_native_app_glue libpng

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
