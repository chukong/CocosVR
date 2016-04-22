LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

ifeq ($(VR_PLATFORM),GEAR_VR)
$(call import-add-path,$(OVRSDKMOBILEROOT))
else ifeq ($(VR_PLATFORM),DEEPOON_VR)
$(call import-add-path,$(DEEPOONSDKROOT))
else ifeq ($(VR_PLATFORM),CARDBOARD_VR)
$(call import-add-path,$(CARDBOARDROOT))
endif

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

ifeq ($(VR_PLATFORM),GEAR_VR)
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   hellocpp/HelperJNI.cpp \
                   ../../../cocosvr/OVRRenderer-gearvr.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/M24.cpp \
                   ../../Classes/P4F2.cpp \
                   ../../Classes/Panther.cpp \
                   ../../Classes/Tank.cpp \
                   ../../Classes/Tiger.cpp \
                   ../../Classes/PlayerController.cpp \
                   ../../Classes/ProgressView.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../../cocosvr \
                    $(OVRSDKMOBILEROOT)VrApi/Include \
                    $(OVRSDKMOBILEROOT)VrAppSupport/SystemUtils/Include
else ifeq ($(VR_PLATFORM),DEEPOON_VR)
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   hellocpp/HelperJNI.cpp \
                   ../../../cocosvr/OVRRenderer-deepoon.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/M24.cpp \
                   ../../Classes/P4F2.cpp \
                   ../../Classes/Panther.cpp \
                   ../../Classes/Tank.cpp \
                   ../../Classes/Tiger.cpp \
                   ../../Classes/PlayerController.cpp \
                   ../../Classes/ProgressView.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../../cocosvr \
                    $(DEEPOONSDKROOT)include
else ifeq ($(VR_PLATFORM),CARDBOARD_VR)
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   hellocpp/HelperJNI.cpp \
                   ../../../cocosvr/cardboard/CbApi.cpp \
                   ../../../cocosvr/OVRRenderer-cardboard.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/M24.cpp \
                   ../../Classes/P4F2.cpp \
                   ../../Classes/Panther.cpp \
                   ../../Classes/Tank.cpp \
                   ../../Classes/Tiger.cpp \
                   ../../Classes/PlayerController.cpp \
                   ../../Classes/ProgressView.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../../cocosvr \
                    $(LOCAL_PATH)/../../../cocosvr/cardboard
endif

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

LOCAL_CPPFLAGS := -Wno-type-limits
LOCAL_CPPFLAGS += -Wno-invalid-offsetof
LOCAL_CPPFLAGS += -std=c++11
LOCAL_CPPFLAGS += -DANDROID

ifeq ($(VR_PLATFORM),GEAR_VR)
LOCAL_STATIC_LIBRARIES += systemutils
LOCAL_STATIC_LIBRARIES += libovrkernel
LOCAL_SHARED_LIBRARIES := vrapi
LOCAL_CPPFLAGS += -DGEAR_VR
else ifeq ($(VR_PLATFORM),DEEPOON_VR)
LOCAL_SHARED_LIBRARIES := deepoon_sdk
LOCAL_CPPFLAGS += -DDEEPOON_VR
else ifeq ($(VR_PLATFORM),CARDBOARD_VR)
LOCAL_CPPFLAGS += -DCARDBOARD_VR
endif

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

ifeq ($(VR_PLATFORM),GEAR_VR)
$(call import-module,LibOVRKernel/Projects/AndroidPrebuilt/jni)
$(call import-module,VrApi/Projects/AndroidPrebuilt/jni)
$(call import-module,VrAppSupport/SystemUtils/Projects/AndroidPrebuilt/jni)
else ifeq ($(VR_PLATFORM),DEEPOON_VR)
$(call import-module,project/jni)
endif

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END