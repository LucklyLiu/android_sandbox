
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := android_sandbox
#一组可选的编译器标志，在编译C和C++源文件的时候会被传送给编译器
LOCAL_CFLAGS := -DANDROID_NDK \
                -DUNIX_SYSTEM \
                -DHPS_CORE_BUILD \
                -DTARGET_OS_ANDROID=1 \
                -DLINUX_SYSTEM \
                -fsigned-char \
                -std=c++11
				
ifeq ($(USING_EXCHANGE),1)
	LOCAL_CFLAGS += -DUSING_EXCHANGE=1
endif

LOCAL_CPP_FEATURES := exceptions

LOCAL_C_INCLUDES += ./jni/shared
LOCAL_C_INCLUDES += ./jni/include


# -- sip --
$(info $(shell python $(TOP_PATH)/../../../../sip/sip.py))
# ---

# --- MobileSurface Base & JNI files ---
LOCAL_SRC_FILES += OnLoadJNI.cpp
LOCAL_SRC_FILES += AndroidMobileSurfaceViewJNI.cpp
LOCAL_SRC_FILES += AndroidUserMobileSurfaceViewJNI.cpp		# Generated
LOCAL_SRC_FILES += MobileAppJNI.cpp							# Generated
LOCAL_SRC_FILES += shared/MobileApp.cpp
LOCAL_SRC_FILES += shared/MobileSurface.cpp
# ---

# --- User files ---
LOCAL_SRC_FILES += shared/UserMobileSurface.cpp
# ---

# Note: Link order below important

ifeq ($(USING_EXCHANGE),1)
	LOCAL_SHARED_LIBRARIES += hps_sprk_exchange
endif
LOCAL_SHARED_LIBRARIES += hps_sprk_ops
LOCAL_SHARED_LIBRARIES += hps_sprk
LOCAL_SHARED_LIBRARIES += hps_core

LOCAL_LDLIBS    := -landroid -llog

include $(BUILD_SHARED_LIBRARY)

