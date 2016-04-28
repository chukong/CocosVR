/************************************************************************************
Filename    :  
Content     :	
Created     :   Dec 2, 2015
Authors     :   deepoon.com
Copyright   :   Copyright 2015 Deepoon, Inc. All Rights reserved.
************************************************************************************/
#ifndef _DEEPOON_SDK_NATIVE_H_
#define _DEEPOON_SDK_NATIVE_H_



// current version 1.0.0
#define DPNN_RUNTIME_VERSION(major,minor,revision)	(0<<(revision))&(0<<(minor))&(major)

// enumerations
typedef enum tag_dpnnUsageMode {
	DPNN_UM_DEFAULT=0,
	DPNN_UM_SENSOR_ONLY=1,
	DPNN_UM_COUNT
} dpnnUsageMode;

typedef enum tag_dpnnDeviceType {
	DPNN_DEVICE_DX11=0,
	DPNN_DEVICE_DX10=1,
	DPNN_DEVICE_DX9=2,
	DPNN_DEVICE_GLES2=3,
	DPNN_DEVICE_COUNT
} dpnnDeviceType;

typedef enum tag_dpnnEyeType {
	DPNN_EYE_LEFT=0,
	DPNN_EYE_RIGHT=1,
	DPNN_EYE_CENTER=2,
	DPNN_EYE_COUNT
} dpnnEyeType;

typedef enum tag_dpnnTwType {
	DPNN_TW_NONE=0,
	DPNN_TW_DISTORTION=1,
	DPNN_TW_TW=2,
	DPNN_TW_TW_DISTORTION=3,
	DPNN_TW_COUNT
} dpnnTwType;

// structure definitions
#pragma pack( 1 )

	typedef void *dpnnInstance;

	typedef struct
	{
		float s;
		float i;
		float j;
		float k;
	} dpnnQuarterion;

	typedef struct
	{
		float x;
		float y;
		float z;
	} dpnnVector3;

	typedef struct
	{
		dpnnVector3 angular_velocity;
		dpnnVector3 linear_acceleration;
		dpnnVector3 magnetometer;
	} dpnnSensorData;
	
	typedef struct
	{
		float ipd;
		int resolution_x;
		int resolution_y;
		float fov_x;
		float fov_y;
	} dpnnDeviceInfo;

#pragma pack()



#ifdef __cplusplus
extern "C" {
#endif

int dpnnGetVersion();
dpnnInstance dpnnInit(int apiVersion, dpnnUsageMode mode, void * device, dpnnDeviceType device_type, void *user_data);
bool dpnnGetDeviceInfo(dpnnInstance  instance, dpnnDeviceInfo *device_info);
bool dpnnSetTexture(dpnnInstance  instance, void *texture, dpnnEyeType eye_type, dpnnTwType time_warping_type);
bool dpnnRecordPose(dpnnInstance instance, dpnnEyeType eye_type );
bool dpnnCompose(dpnnInstance instance);
bool dpnnDeinit(dpnnInstance instance);
const dpnnQuarterion dpnnGetPose(dpnnInstance  instance);
const dpnnVector3 dpnnGetPosition(dpnnInstance instance);
bool dpnnGetSensorData(dpnnInstance instance, dpnnSensorData * const data);
bool dpnnVerifySignature(void *signiture_data, unsigned int signiture_len);
void * dpnnGetUserData(dpnnInstance instance);

#ifdef __cplusplus
}
#endif




#endif /* _DEEPOON_SDK_NATIVE_H_ */
