LOCAL_PATH := $(call my-dir)

##############################

include $(CLEAR_VARS)

LOCAL_MODULE    := OGLESSample_NDK
LOCAL_SRC_FILES := main.cpp kernel.cpp android_task.cpp android_application.cpp renderer.cpp egl_renderer.cpp timer_task.cpp renderable.cpp gl_renderable.cpp scene.cpp gl_scene.cpp
LOCAL_LDLIBS 	:=-llog -landroid -lEGL -lGLESv2
				  # for GLESv1, use -lGLESv1_CM
LOCAL_STATIC_LIBRARIES	:=android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/native_app_glue)
##############################
