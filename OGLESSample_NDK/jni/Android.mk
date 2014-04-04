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
#LOCAL_CFLAGS    += -DGLDEBUG
#LOCAL_CFLAGS    += -D_GDB_DEBUG_
LOCAL_LDLIBS 	:= -llog -landroid -lEGL -lGLESv2 -lz
				  # for GLES 1.x, use -lGLESv1_CM
LOCAL_STATIC_LIBRARIES	:= android_native_app_glue libpng

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
