#ifndef __OVR_RENDERER_H__
#define __OVR_RENDERER_H__

#ifdef OCULUS_VR
#include "OVRRenderer-oculus.h"
#elif GEAR_VR
#include "OVRRenderer-gearvr.h"
#elif DEEPOON_VR
#include "OVRRenderer-deepoon.h"
#elif CARDBOARD_VR
#include "OVRRenderer-cardboard.h"
#endif

#endif