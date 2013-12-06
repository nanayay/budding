LOCAL_PATH := $(call my-dir)

##############################

include $(CLEAR_VARS)

LOCAL_MODULE    := OGLSample
LOCAL_SRC_FILES := OGLSample.cpp main.cpp kernel.cpp android_task.cpp renderer.cpp egl_renderer.cpp
LOCAL_LDLIBS 	:=-llog -landroid -lEGL -lGLESv2
				  # for GLESv1, use -lGLESv1_CM
LOCAL_STATIC_LIBRARIES	:=android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
