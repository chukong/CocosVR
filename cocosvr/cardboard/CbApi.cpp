#include "CbApi.h"
static JavaVM * g_Vm  = nullptr;
static JNIEnv * g_Env = nullptr;

bool cbapi_Initialize(const cbJava *java, cbHeadMountedDisplay *headMountedDisplay)
{
    bool status = false;
    g_Vm = java->Vm;
    g_Env = java->Env;
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");

    do{
        if (!cls) break;
        jmethodID mid = g_Env->GetStaticMethodID(cls, "initialize", "(Landroid/content/Context;[F)V");
        if (!mid) break;
        jfloatArray params = g_Env->NewFloatArray(13);
        g_Env->CallStaticVoidMethod(cls, mid, java->ActivityObject, params);

        jfloat* paramsfPtr = g_Env->GetFloatArrayElements(params, 0);

        headMountedDisplay->screen.width            = paramsfPtr[0];
        headMountedDisplay->screen.height           = paramsfPtr[1];
        headMountedDisplay->screen.xMeters          = paramsfPtr[2];
        headMountedDisplay->screen.yMeters          = paramsfPtr[3];
        headMountedDisplay->screen.borderSizeMeters = paramsfPtr[4];

        headMountedDisplay->device.interLensDistance = paramsfPtr[5];
        if (paramsfPtr[6] == 0.0){
            headMountedDisplay->device.VerticalAlignment = CBAPI_VERTICAL_ALIGNMENT_BOTTOM;
        }else if (paramsfPtr[6] == 1.0){
            headMountedDisplay->device.VerticalAlignment = CBAPI_VERTICAL_ALIGNMENT_CENTER;
        }else{
            headMountedDisplay->device.VerticalAlignment = CBAPI_VERTICAL_ALIGNMENT_TOP;
        }
        headMountedDisplay->device.verticalDistanceToLensCenter = paramsfPtr[7];
        headMountedDisplay->device.screenToLensDistance         = paramsfPtr[8];

        headMountedDisplay->device.leftEyeMaxFov.left   = paramsfPtr[9];
        headMountedDisplay->device.leftEyeMaxFov.right  = paramsfPtr[10];
        headMountedDisplay->device.leftEyeMaxFov.bottom = paramsfPtr[11];
        headMountedDisplay->device.leftEyeMaxFov.top    = paramsfPtr[12];

        g_Env->ReleaseFloatArrayElements(params, paramsfPtr, 0);

        status = true;
    }while(false);

    return status;
}

cbEyes cbapi_GetEyes()
{
    cbEyes eyes;

    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (cls){
        jmethodID mid = g_Env->GetStaticMethodID(cls, "getEyes", "([F)V");

        jfloatArray params = g_Env->NewFloatArray(16);
        g_Env->CallStaticVoidMethod(cls, mid, params);

        jfloat* paramsfPtr = g_Env->GetFloatArrayElements(params, 0);

        eyes.eyeParams[0].viewport.x      = (int)paramsfPtr[0];
        eyes.eyeParams[0].viewport.y      = (int)paramsfPtr[1];
        eyes.eyeParams[0].viewport.width  = (int)paramsfPtr[2];
        eyes.eyeParams[0].viewport.height = (int)paramsfPtr[3];
        eyes.eyeParams[0].fov.left        = paramsfPtr[4];
        eyes.eyeParams[0].fov.right       = paramsfPtr[5];
        eyes.eyeParams[0].fov.bottom      = paramsfPtr[6];
        eyes.eyeParams[0].fov.top         = paramsfPtr[7];

        eyes.eyeParams[1].viewport.x      = (int)paramsfPtr[8];
        eyes.eyeParams[1].viewport.y      = (int)paramsfPtr[9];
        eyes.eyeParams[1].viewport.width  = (int)paramsfPtr[10];
        eyes.eyeParams[1].viewport.height = (int)paramsfPtr[11];
        eyes.eyeParams[1].fov.left        = paramsfPtr[12];
        eyes.eyeParams[1].fov.right       = paramsfPtr[13];
        eyes.eyeParams[1].fov.bottom      = paramsfPtr[14];
        eyes.eyeParams[1].fov.top         = paramsfPtr[15];

        g_Env->ReleaseFloatArrayElements(params, paramsfPtr, 0);
    }

    return eyes;
}

void cbapi_EnterVrMode()
{
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (!cls) return;
    jmethodID mid = g_Env->GetStaticMethodID(cls, "enterVrMode", "()V");
    g_Env->CallStaticVoidMethod(cls, mid);
}

void cbapi_LeaveVrMode()
{
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (!cls) return;
    jmethodID mid = g_Env->GetStaticMethodID(cls, "leaveVrMode", "()V");
    g_Env->CallStaticVoidMethod(cls, mid);
}

cbMatrix4 cbapi_getLastHeadView()
{
    cbMatrix4 headView;
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (cls){
        jmethodID mid = g_Env->GetStaticMethodID(cls, "getLastHeadView", "([F)V");
        jfloatArray params = g_Env->NewFloatArray(16);
        g_Env->CallStaticVoidMethod(cls, mid, params);

        jfloat* paramsfPtr = g_Env->GetFloatArrayElements(params, 0);

        headView.M[0][0] = paramsfPtr[0];
        headView.M[1][0] = paramsfPtr[1];
        headView.M[2][0] = paramsfPtr[2];
        headView.M[3][0] = paramsfPtr[3];

        headView.M[0][1] = paramsfPtr[4];
        headView.M[1][1] = paramsfPtr[5];
        headView.M[2][1] = paramsfPtr[6];
        headView.M[3][1] = paramsfPtr[7];

        headView.M[0][2] = paramsfPtr[8];
        headView.M[1][2] = paramsfPtr[9];
        headView.M[2][2] = paramsfPtr[10];
        headView.M[3][2] = paramsfPtr[11];

        headView.M[0][3] = paramsfPtr[12];
        headView.M[1][3] = paramsfPtr[13];
        headView.M[2][3] = paramsfPtr[14];
        headView.M[3][3] = paramsfPtr[15];

        g_Env->ReleaseFloatArrayElements(params, paramsfPtr, 0);
    }

    return headView;
}

void cbapi_beforeDrawFrame()
{
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (!cls) return;
    jmethodID mid = g_Env->GetStaticMethodID(cls, "beforeDrawFrame", "()V");
    g_Env->CallStaticVoidMethod(cls, mid);
}

void cbapi_afterDrawFrame()
{
    jclass cls = g_Env->FindClass("org/cocos2dx/cpp/CbAPIWrapper");
    if (!cls) return;
    jmethodID mid = g_Env->GetStaticMethodID(cls, "afterDrawFrame", "()V");
    g_Env->CallStaticVoidMethod(cls, mid);
}