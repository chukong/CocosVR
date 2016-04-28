LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := deepoon_sdk

LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).so

# only export public headers
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

ifneq (,$(wildcard $(LOCAL_PATH)/$(LOCAL_SRC_FILES)))
  include $(PREBUILT_SHARED_LIBRARY)
endif
