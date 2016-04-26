/************************************************************************************
Filename    :  
Content     :	
Created     :   June 8, 2015
Authors     :   deepoon.com
Copyright   :   Copyright 2015 Deepoon, Inc. All Rights reserved.
************************************************************************************/

/// @brief 
/// @param [in/out] p0 
/// @return
/// @retval 0
/// @retval 1
/// @note
/// @remarks
/// @author
/// @version
/// @file
/// @date
#include <jni.h>

#ifndef _DEEPOON_DEEPOON_SDK_H_
#define _DEEPOON_DEEPOON_SDK_H_

/************************************************************************************
global defines
************************************************************************************/
#define DEEPOON_SDK_MAJOR_VERSION		0
#define DEEPOON_SDK_MINOR_VERSION		1

#define DEEPOON_SDK_ENABLE_EXPORT

#if defined( DEEPOON_SDK_ENABLE_EXPORT )
    #define DEEPOON_SDK_EXPORT __attribute__((__visibility__("default")))
#else
    #define DEEPOON_SDK_EXPORT 
#endif

#define DEEPOON_SDK_DEPRECATED __attribute__ ((deprecated))

/************************************************************************************
deepoon enums
************************************************************************************/
// Bit flags describing the current status of sensor tracking.
typedef enum
{
    dpnSensorStatus_OrientationTracked    = 0x0001,   // Orientation is currently tracked (connected and in use).
    dpnSensorStatus_PositionTracked       = 0x0002,   // Position is currently tracked (FALSE if out of range).
    dpnSensorStatus_PositionConnected     = 0x0020,   // Position tracking HW is connected.
    dpnSensorStatus_HmdConnected          = 0x0080    // HMD Display is available & connected.
} dpnSensorStatusBits;

typedef enum
{
    DPNFRAME_EYE_LEFT,
    DPNFRAME_EYE_RIGHT,
    DPNFRAME_EYE_MAX
} dpnFrameEyeType;

typedef enum
{
    DPNFRAME_LAYER_TYPE_WORLD,
    DPNFRAME_LAYER_TYPE_OVERLAY,
    DPNFRAME_LAYER_TYPE_CURSOR,
    DPNFRAME_LAYER_TYPE_MAX
} dpnFrameLayerType;

/************************************************************************************
basic structs
************************************************************************************/

typedef struct
{
	int x;
	int y;
	int w;
	int h;
} dpnRect;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} dpnQuarterion;

typedef struct
{
	float x;
	float y;
} dpnVector2;

typedef struct
{
	float x;
	float y;
	float z;
} dpnVector3;

typedef struct
{
    float M[4][4];
} dpnMatrix4;

typedef struct
{
	dpnQuarterion q;
	dpnVector3 p;
} dpnPose;

typedef struct
{
	unsigned int status;
    dpnPose pose;
	dpnVector3 angularVelocity;
	dpnVector3 linearAcceleration;
	dpnVector3 magnetometer;
    double timeSeconds;
} dpnSensorData;

typedef struct
{
	unsigned int texId;
	unsigned int planarTexId[3];
	dpnSensorData pose;
} dpnFrameImage;

typedef struct
{
	dpnFrameImage image[DPNFRAME_EYE_MAX][DPNFRAME_LAYER_TYPE_MAX];
} dpnFrame;

typedef struct
{
	int eyeResolution[2];
	float eyeFov[2];
} dpnDeviceInfo;

typedef struct
{
	float ipd;
	float eyeHeight; // Eye height relative to the ground.
	float depth; // Eye offset forward from the head center at EyeHeight.
	float height; // Neck joint offset down from the head center at EyeHeight.
} dpnHmdParms;

typedef struct
{
    int majorVersion;
    int minorVersion;
    void *context;
    dpnDeviceInfo devInfo;
} dpnContext;

/************************************************************************************
dynamic load deepoon
************************************************************************************/

extern "C"
{
DEEPOON_SDK_EXPORT dpnContext * dpnsdkInit(jobject activity);
DEEPOON_SDK_EXPORT void dpnsdkUninit(dpnContext *ctx);
DEEPOON_SDK_EXPORT void dpnsdkGetPose(dpnContext *ctx, dpnSensorData *sensor);
DEEPOON_SDK_EXPORT void dpnsdkFrame(dpnContext *ctx, dpnFrame *frm);
}
#endif
