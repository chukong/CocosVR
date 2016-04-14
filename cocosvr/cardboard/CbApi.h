#ifndef _CbApi_h
#define _CbApi_h

#include "CbApi_Config.h"
#include "CbApi_Types.h"

#if defined( __cplusplus )
extern "C" {
#endif

CBAPI_EXPORT cbHeadMountedDisplay cbapi_Initialize(const cbJava *java);
CBAPI_EXPORT cbEyes cbapi_EnterVrMode();
CBAPI_EXPORT void cbapi_LeaveVrMode();
CBAPI_EXPORT cbMatrix4 cbapi_getLastHeadView();
CBAPI_EXPORT void cbapi_beforeDrawFrame();
CBAPI_EXPORT void cbapi_afterDrawFrame();

#if defined( __cplusplus )
}	// extern "C"
#endif

#endif	// OVR_VrApi_h
