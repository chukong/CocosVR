#ifndef _CbApi_Types_h
#define _CbApi_Types_h

#include <stdbool.h>
#include "CbApi_Config.h"

//-----------------------------------------------------------------
// Java
//-----------------------------------------------------------------

#if defined( ANDROID )
#include <jni.h>
#elif defined( __cplusplus )
typedef struct _JNIEnv JNIEnv;
typedef struct _JavaVM JavaVM;
typedef class _jobject * jobject;
#else
typedef const struct JNINativeInterface * JNIEnv;
typedef const struct JNIInvokeInterface * JavaVM;
void * jobject;
#endif

typedef struct
{
    JavaVM *	Vm;					// Java Virtual Machine
    JNIEnv *	Env;				// Thread specific environment
    jobject		ActivityObject;		// Java activity object
} cbJava;

#define CB_EYE_NUM 2

typedef struct cbVector3_
{
    float x, y, z;
} cbVector3;

// Quaternion.
typedef struct cbQuat_
{
    float x, y, z, w;
} cbQuat;

// row-major 4x4 matrix.
typedef struct cbMatrix4_
{
    float M[4][4];
} cbMatrix4;

typedef struct cbViewport_
{
    int x;
    int y;
    int width;
    int height;
} cbViewport;

typedef struct cbFieldOfView_
{
    float left;
    float right;
    float bottom;
    float top;
} cbFieldOfView;

typedef struct cbEyeParams_
{
    cbViewport    viewport;
    cbFieldOfView fov;
} cbEyeParams;

typedef struct cbEyes_
{
    cbEyeParams eyeParams[CB_EYE_NUM];
} cbEyes;

typedef enum
{
    CBAPI_FALSE = 0,
    CBAPI_TRUE
} cbBooleanResult;

typedef enum
{
    CBAPI_VERTICAL_ALIGNMENT_BOTTOM = 0,
    CBAPI_VERTICAL_ALIGNMENT_CENTER,
    CBAPI_VERTICAL_ALIGNMENT_TOP,
} cbVerticalAlignmentType;

typedef struct cbScreenParams_
{
    int   width;
    int   height;
    float xMeters;
    float yMeters;
    float borderSizeMeters;
} cbScreenParams;

typedef struct cbDeviceParams_
{
    float                   interLensDistance;
    cbVerticalAlignmentType VerticalAlignment;
    float                   verticalDistanceToLensCenter;
    float                   screenToLensDistance;
    cbFieldOfView           leftEyeMaxFov;
} cbDeviceParams;

typedef struct cbHeadMountedDisplay_
{
    cbScreenParams screen;
    cbDeviceParams device;
} cbHeadMountedDisplay;

#endif
