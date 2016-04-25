#ifndef _CbApi_h
#define _CbApi_h

#include "CbApi_Config.h"
#include "CbApi_Types.h"

#if defined( __cplusplus )
extern "C" {
#endif

CBAPI_EXPORT bool cbapi_Initialize(const cbJava *java, cbHeadMountedDisplay *headMountedDisplay);
CBAPI_EXPORT cbEyes cbapi_GetEyes();
CBAPI_EXPORT void cbapi_EnterVrMode();
CBAPI_EXPORT void cbapi_LeaveVrMode();
CBAPI_EXPORT cbMatrix4 cbapi_getLastHeadView();
CBAPI_EXPORT void cbapi_beforeDrawFrame();
CBAPI_EXPORT void cbapi_afterDrawFrame();

#if defined( __cplusplus )
}	// extern "C"
#endif

#endif
