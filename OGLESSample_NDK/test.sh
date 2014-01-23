echo "Begin test on device or emulator"

ant uninstall OGLSample;ndk-build&&ant debug&&ant installd