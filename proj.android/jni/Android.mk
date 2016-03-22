LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(OVRSDKMOBILEROOT))

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   hellocpp/HelperJNI.cpp \
				   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/M24.cpp \
                   ../../Classes/P4F2.cpp \
                   ../../Classes/Panther.cpp \
                   ../../Classes/Tank.cpp \
                   ../../Classes/Tiger.cpp \
                   ../../Classes/OVRRenderer-android.cpp \
                   ../../Classes/OVRHelper-android.cpp \
                   ../../Classes/PlayerController.cpp \
                   ../../Classes/ProgressView.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(OVRSDKMOBILEROOT)VrApi/Include \
					$(OVRSDKMOBILEROOT)VrAppSupport/SystemUtils/Include

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += systemutils
LOCAL_STATIC_LIBRARIES += libovrkernel

LOCAL_SHARED_LIBRARIES := vrapi

LOCAL_CPPFLAGS := -Wno-type-limits
LOCAL_CPPFLAGS += -Wno-invalid-offsetof
LOCAL_CPPFLAGS += -std=c++11
LOCAL_CPPFLAGS += -DANDROID

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,LibOVRKernel/Projects/AndroidPrebuilt/jni)
$(call import-module,VrApi/Projects/AndroidPrebuilt/jni)
$(call import-module,VrAppSupport/SystemUtils/Projects/AndroidPrebuilt/jni)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
