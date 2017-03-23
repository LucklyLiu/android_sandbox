LOCAL_PATH := $(call my-dir)

#MY_USE_DEBUG_HPS_LIBS := 1

ifeq ($(MY_USE_DEBUG_HPS_LIBS),1)
	MY_HPS_DEBUG_SUFFIX := d
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	MY_HPS_ARCH_SUFFIX := _arm32
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	MY_HPS_ARCH_SUFFIX := _arm64
endif

MY_HPS_LIB_PATH  := ./libs$(MY_HPS_ARCH_SUFFIX)


include $(CLEAR_VARS)
LOCAL_MODULE := hps_core
LOCAL_SRC_FILES := $(MY_HPS_LIB_PATH)/libhps_core.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := hps_sprk
LOCAL_SRC_FILES := $(MY_HPS_LIB_PATH)/libhps_sprk.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := hps_sprk_ops
LOCAL_SRC_FILES := $(MY_HPS_LIB_PATH)/libhps_sprk_ops.so
include $(PREBUILT_SHARED_LIBRARY)

ifeq ($(USING_EXCHANGE),1)
    include $(CLEAR_VARS)
    LOCAL_MODULE := hps_sprk_exchange
    LOCAL_SRC_FILES := $(MY_HPS_LIB_PATH)/libhps_sprk_exchange.so
    include $(PREBUILT_SHARED_LIBRARY)

    include $(CLEAR_VARS)
    LOCAL_MODULE := A3DLIBS
    HEXCHANGE_INSTALL_DIR := $(subst \,/,$(HEXCHANGE_INSTALL_DIR))
    HEXCHANGE_LIB_DIR := $(HEXCHANGE_INSTALL_DIR)/bin/android/$(TARGET_ARCH_ABI)
	
	LOCAL_SRC_FILES := $(HEXCHANGE_LIB_DIR)/libA3DLIBS.so
	include $(PREBUILT_SHARED_LIBRARY)
endif
