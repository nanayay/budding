APP_ABI := armeabi-v7a x86 armeabi
APP_PLATFORM := android-18

# APP_OPTIM := debug 
# APP_OPTIM := release

# use gnustl_static or gnustl_share, you need to add -frtti for ignore compile error
# use stlport_static or stlport_share, you didn't need to add -frtti, still can use dynamic_cast
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -fexceptions
APP_CPPFLAGS += -std=c++11
NDK_TOOLCHAIN_VERSION := 4.8

