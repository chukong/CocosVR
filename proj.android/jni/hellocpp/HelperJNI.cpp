#include <jni.h>

void cocos_android_app_onCreate(JNIEnv* env, jobject activity) __attribute__((weak));

extern "C" {

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_nativeOnCreate(JNIEnv*  env, jobject thiz, jobject activity) {
		cocos_android_app_onCreate(env, activity);
	}
}