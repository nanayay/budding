TOP_PATH := $(call my-dir)

##############################
# For libpng
include $(CLEAR_VARS)

include $(TOP_PATH)/external/libpng/jni/Android.mk

LOCAL_PATH := $(TOP_PATH)

##############################
# For OGLESSample_NDK
include $(CLEAR_VARS)

LOCAL_MODULE    := OGLESSample_NDK
LOCAL_SRC_FILES := main.cpp kernel.cpp android_task.cpp android_application.cpp renderer.cpp egl_renderer.cpp timer_task.cpp renderable.cpp gl_renderable.cpp scene.cpp gl_scene.cpp android_asset.cpp
LOCAL_CFLAGS    := 
LOCAL_LDLIBS 	:= -llog -landroid -lEGL -lGLESv2 -lz
				  # for GLES 1.x, use -lGLESv1_CM
LOCAL_STATIC_LIBRARIES	:= android_native_app_glue libpng

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
