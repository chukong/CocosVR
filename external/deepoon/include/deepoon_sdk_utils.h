/************************************************************************************

Filename    :   deepoon_sdk_utils.h

Copyright   :   Copyright 2015 DeePoon VR, LLC. All Rights reserved.

*************************************************************************************/
#ifndef DEEPOON_SDK_UTILS_H
#define DEEPOON_SDK_UTILS_H

#include <math.h>
#include <string.h>

#include "deepoon_sdk.h"

//-----------------------------------------------------------------
// Matrix helper functions
//-----------------------------------------------------------------

// Use left-multiplication to accumulate transformations.
inline dpnMatrix4 dpnutilMatrix4_Multiply( const dpnMatrix4 * a, const dpnMatrix4 * b )
{
	dpnMatrix4 out;
	out.M[0][0] = a->M[0][0] * b->M[0][0] + a->M[0][1] * b->M[1][0] + a->M[0][2] * b->M[2][0] + a->M[0][3] * b->M[3][0];
	out.M[1][0] = a->M[1][0] * b->M[0][0] + a->M[1][1] * b->M[1][0] + a->M[1][2] * b->M[2][0] + a->M[1][3] * b->M[3][0];
	out.M[2][0] = a->M[2][0] * b->M[0][0] + a->M[2][1] * b->M[1][0] + a->M[2][2] * b->M[2][0] + a->M[2][3] * b->M[3][0];
	out.M[3][0] = a->M[3][0] * b->M[0][0] + a->M[3][1] * b->M[1][0] + a->M[3][2] * b->M[2][0] + a->M[3][3] * b->M[3][0];

	out.M[0][1] = a->M[0][0] * b->M[0][1] + a->M[0][1] * b->M[1][1] + a->M[0][2] * b->M[2][1] + a->M[0][3] * b->M[3][1];
	out.M[1][1] = a->M[1][0] * b->M[0][1] + a->M[1][1] * b->M[1][1] + a->M[1][2] * b->M[2][1] + a->M[1][3] * b->M[3][1];
	out.M[2][1] = a->M[2][0] * b->M[0][1] + a->M[2][1] * b->M[1][1] + a->M[2][2] * b->M[2][1] + a->M[2][3] * b->M[3][1];
	out.M[3][1] = a->M[3][0] * b->M[0][1] + a->M[3][1] * b->M[1][1] + a->M[3][2] * b->M[2][1] + a->M[3][3] * b->M[3][1];

	out.M[0][2] = a->M[0][0] * b->M[0][2] + a->M[0][1] * b->M[1][2] + a->M[0][2] * b->M[2][2] + a->M[0][3] * b->M[3][2];
	out.M[1][2] = a->M[1][0] * b->M[0][2] + a->M[1][1] * b->M[1][2] + a->M[1][2] * b->M[2][2] + a->M[1][3] * b->M[3][2];
	out.M[2][2] = a->M[2][0] * b->M[0][2] + a->M[2][1] * b->M[1][2] + a->M[2][2] * b->M[2][2] + a->M[2][3] * b->M[3][2];
	out.M[3][2] = a->M[3][0] * b->M[0][2] + a->M[3][1] * b->M[1][2] + a->M[3][2] * b->M[2][2] + a->M[3][3] * b->M[3][2];

	out.M[0][3] = a->M[0][0] * b->M[0][3] + a->M[0][1] * b->M[1][3] + a->M[0][2] * b->M[2][3] + a->M[0][3] * b->M[3][3];
	out.M[1][3] = a->M[1][0] * b->M[0][3] + a->M[1][1] * b->M[1][3] + a->M[1][2] * b->M[2][3] + a->M[1][3] * b->M[3][3];
	out.M[2][3] = a->M[2][0] * b->M[0][3] + a->M[2][1] * b->M[1][3] + a->M[2][2] * b->M[2][3] + a->M[2][3] * b->M[3][3];
	out.M[3][3] = a->M[3][0] * b->M[0][3] + a->M[3][1] * b->M[1][3] + a->M[3][2] * b->M[2][3] + a->M[3][3] * b->M[3][3];
	return out;
}

inline dpnMatrix4 dpnutilMatrix4_Transpose( const dpnMatrix4 * a )
{
	dpnMatrix4 out;
	out.M[0][0] = a->M[0][0]; out.M[0][1] = a->M[1][0]; out.M[0][2] = a->M[2][0]; out.M[0][3] = a->M[3][0];
	out.M[1][0] = a->M[0][1]; out.M[1][1] = a->M[1][1]; out.M[1][2] = a->M[2][1]; out.M[1][3] = a->M[3][1];
	out.M[2][0] = a->M[0][2]; out.M[2][1] = a->M[1][2]; out.M[2][2] = a->M[2][2]; out.M[2][3] = a->M[3][2];
	out.M[3][0] = a->M[0][3]; out.M[3][1] = a->M[1][3]; out.M[3][2] = a->M[2][3]; out.M[3][3] = a->M[3][3];
	return out;
}

// Returns a 3x3 minor of a 4x4 matrix.
inline float dpnutilMatrix4_Minor( const dpnMatrix4 * m, int r0, int r1, int r2, int c0, int c1, int c2 )
{
	return	m->M[r0][c0] * ( m->M[r1][c1] * m->M[r2][c2] - m->M[r2][c1] * m->M[r1][c2] ) -
			m->M[r0][c1] * ( m->M[r1][c0] * m->M[r2][c2] - m->M[r2][c0] * m->M[r1][c2] ) +
			m->M[r0][c2] * ( m->M[r1][c0] * m->M[r2][c1] - m->M[r2][c0] * m->M[r1][c1] );
}
 
// Returns the inverse of a 4x4 matrix.
inline dpnMatrix4 dpnutilMatrix4_Inverse( const dpnMatrix4 * m )
{
	const float rcpDet = 1.0f / (	m->M[0][0] * dpnutilMatrix4_Minor( m, 1, 2, 3, 1, 2, 3 ) -
									m->M[0][1] * dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 2, 3 ) +
									m->M[0][2] * dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 1, 3 ) -
									m->M[0][3] * dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 1, 2 ) );
	dpnMatrix4 out;
	out.M[0][0] =  dpnutilMatrix4_Minor( m, 1, 2, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][1] = -dpnutilMatrix4_Minor( m, 0, 2, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][2] =  dpnutilMatrix4_Minor( m, 0, 1, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][3] = -dpnutilMatrix4_Minor( m, 0, 1, 2, 1, 2, 3 ) * rcpDet;
	out.M[1][0] = -dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][1] =  dpnutilMatrix4_Minor( m, 0, 2, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][2] = -dpnutilMatrix4_Minor( m, 0, 1, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][3] =  dpnutilMatrix4_Minor( m, 0, 1, 2, 0, 2, 3 ) * rcpDet;
	out.M[2][0] =  dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][1] = -dpnutilMatrix4_Minor( m, 0, 2, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][2] =  dpnutilMatrix4_Minor( m, 0, 1, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][3] = -dpnutilMatrix4_Minor( m, 0, 1, 2, 0, 1, 3 ) * rcpDet;
	out.M[3][0] = -dpnutilMatrix4_Minor( m, 1, 2, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][1] =  dpnutilMatrix4_Minor( m, 0, 2, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][2] = -dpnutilMatrix4_Minor( m, 0, 1, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][3] =  dpnutilMatrix4_Minor( m, 0, 1, 2, 0, 1, 2 ) * rcpDet;
	return out;
}

inline dpnMatrix4 dpnutilMatrix4_CreateIdentity()
{
	dpnMatrix4 out;
	out.M[0][0] = 1.0f; out.M[0][1] = 0.0f; out.M[0][2] = 0.0f; out.M[0][3] = 0.0f;
	out.M[1][0] = 0.0f; out.M[1][1] = 1.0f; out.M[1][2] = 0.0f; out.M[1][3] = 0.0f;
	out.M[2][0] = 0.0f; out.M[2][1] = 0.0f; out.M[2][2] = 1.0f; out.M[2][3] = 0.0f;
	out.M[3][0] = 0.0f; out.M[3][1] = 0.0f; out.M[3][2] = 0.0f; out.M[3][3] = 1.0f;
	return out;
}

inline dpnMatrix4 dpnutilMatrix4_CreateTranslation( const float x, const float y, const float z )
{
	dpnMatrix4 out;
	out.M[0][0] = 1.0f; out.M[0][1] = 0.0f; out.M[0][2] = 0.0f; out.M[0][3] = x;
	out.M[1][0] = 0.0f; out.M[1][1] = 1.0f; out.M[1][2] = 0.0f; out.M[1][3] = y;
	out.M[2][0] = 0.0f; out.M[2][1] = 0.0f; out.M[2][2] = 1.0f; out.M[2][3] = z;
	out.M[3][0] = 0.0f; out.M[3][1] = 0.0f; out.M[3][2] = 0.0f; out.M[3][3] = 1.0f;
	return out;
}

inline dpnMatrix4 dpnutilMatrix4_CreateRotation( const float radiansX, const float radiansY, const float radiansZ )
{
    const float sinX = sinf( radiansX );
    const float cosX = cosf( radiansX );
	const dpnMatrix4 rotationX =
	{ {
		{ 1,    0,     0, 0 },
		{ 0, cosX, -sinX, 0 },
		{ 0, sinX,  cosX, 0 },
		{ 0,    0,     0, 1 }
	} };
    const float sinY = sinf( radiansY );
    const float cosY = cosf( radiansY );
	const dpnMatrix4 rotationY =
	{ {
		{  cosY, 0, sinY, 0 },
		{     0, 1,    0, 0 },
		{ -sinY, 0, cosY, 0 },
		{     0, 0,    0, 1 }
	} };
    const float sinZ = sinf( radiansZ );
    const float cosZ = cosf( radiansZ );
	const dpnMatrix4 rotationZ =
	{ {
		{ cosZ, -sinZ, 0, 0 },
		{ sinZ,  cosZ, 0, 0 },
		{    0,     0, 1, 0 },
		{    0,     0, 0, 1 }
	} };
	const dpnMatrix4 rotationXY = dpnutilMatrix4_Multiply( &rotationY, &rotationX );
	return dpnutilMatrix4_Multiply( &rotationZ, &rotationXY );
}

inline dpnMatrix4 dpnutilMatrix4_CreateProjection( const float minX, const float maxX,
											float const minY, const float maxY, const float nearZ, const float farZ )
{
	const float width = maxX - minX;
	const float height = maxY - minY;
	const float offsetZ = nearZ;	// set to zero for a [0,1] clip space

	dpnMatrix4 out;
	if ( farZ <= nearZ )
	{
		// place the far plane at infinity
		out.M[0][0] = 2 * nearZ / width;
		out.M[0][1] = 0;
		out.M[0][2] = ( maxX + minX ) / width;
		out.M[0][3] = 0;

		out.M[1][0] = 0;
		out.M[1][1] = 2 * nearZ / height;
		out.M[1][2] = ( maxY + minY ) / height;
		out.M[1][3] = 0;

		out.M[2][0] = 0;
		out.M[2][1] = 0;
		out.M[2][2] = -1;
		out.M[2][3] = -( nearZ + offsetZ );

		out.M[3][0] = 0;
		out.M[3][1] = 0;
		out.M[3][2] = -1;
		out.M[3][3] = 0;
	}
	else
	{
		// normal projection
		out.M[0][0] = 2 * nearZ / width;
		out.M[0][1] = 0;
		out.M[0][2] = ( maxX + minX ) / width;
		out.M[0][3] = 0;

		out.M[1][0] = 0;
		out.M[1][1] = 2 * nearZ / height;
		out.M[1][2] = ( maxY + minY ) / height;
		out.M[1][3] = 0;

		out.M[2][0] = 0;
		out.M[2][1] = 0;
		out.M[2][2] = -( farZ + offsetZ ) / ( farZ - nearZ );
		out.M[2][3] = -( farZ * ( nearZ + offsetZ ) ) / ( farZ - nearZ );

		out.M[3][0] = 0;
		out.M[3][1] = 0;
		out.M[3][2] = -1;
		out.M[3][3] = 0;
	}
	return out;
}

inline dpnMatrix4 dpnutilMatrix4_CreateProjectionFov( const float fovRadiansX, const float fovRadiansY,
												const float offsetX, const float offsetY, const float nearZ, const float farZ )
{
	const float halfWidth = nearZ * tanf( fovRadiansX * 0.5f );
	const float halfHeight = nearZ * tanf( fovRadiansY * 0.5f );

	const float minX = offsetX - halfWidth;
	const float maxX = offsetX + halfWidth;

	const float minY = offsetY - halfHeight;
	const float maxY = offsetY + halfHeight;

	return dpnutilMatrix4_CreateProjection( minX, maxX, minY, maxY, nearZ, farZ );
}

// Returns the 4x4 rotation matrix for the given quaternion.
inline dpnMatrix4 dpnutilMatrix4_CreateFromQuaternion( const dpnQuarterion * q )
{
	const float ww = q->w * q->w;
	const float xx = q->x * q->x;
	const float yy = q->y * q->y;
	const float zz = q->z * q->z;

	dpnMatrix4 out;
	out.M[0][0] = ww + xx - yy - zz;
	out.M[0][1] = 2 * ( q->x * q->y - q->w * q->z );
	out.M[0][2] = 2 * ( q->x * q->z + q->w * q->y );
	out.M[0][3] = 0;
	out.M[1][0] = 2 * ( q->x * q->y + q->w * q->z );
	out.M[1][1] = ww - xx + yy - zz;
	out.M[1][2] = 2 * ( q->y * q->z - q->w * q->x );
	out.M[1][3] = 0;
	out.M[2][0] = 2 * ( q->x * q->z - q->w * q->y );
	out.M[2][1] = 2 * ( q->y * q->z + q->w * q->x );
	out.M[2][2] = ww - xx - yy + zz;
	out.M[2][3] = 0;
	out.M[3][0] = 0;
	out.M[3][1] = 0;
	out.M[3][2] = 0;
	out.M[3][3] = 1;
	return out;
}

//-----------------------------------------------------------------
// dpnTimeWarpParms initialization helper functions
//-----------------------------------------------------------------

// Convert a standard projection matrix into a TanAngle matrix for
// the primary time warp surface.
inline dpnMatrix4 dpnutilMatrix4_TanAngleMatrixFromProjection( const dpnMatrix4 * projection )
{
	// A projection matrix goes from a view point to NDC, or -1 to 1 space.
	// Scale and bias to convert that to a 0 to 1 space.
	const dpnMatrix4 tanAngleMatrix =
	{ {
		{ 0.5f * projection->M[0][0], 0.5f * projection->M[0][1], 0.5f * projection->M[0][2] - 0.5f, 0.5f * projection->M[0][3] },
		{ 0.5f * projection->M[1][0], 0.5f * projection->M[1][1], 0.5f * projection->M[1][2] - 0.5f, 0.5f * projection->M[1][3] },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f }
	} };
	return tanAngleMatrix;
}

// Trivial version of TanAngleMatrixFromProjection() for a symmetric field of view.
inline dpnMatrix4 dpnutilMatrix4_TanAngleMatrixFromFov( const float fovDegrees )
{
	const float tanHalfFov = tanf( 0.5f * fovDegrees * ( M_PI / 180.0f ) );
	const dpnMatrix4 tanAngleMatrix =
	{ {
		{ 0.5f / tanHalfFov, 0.0f, -0.5f, 0.0f },
		{ 0.0f, 0.5f / tanHalfFov, -0.5f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f }
	} };
	return tanAngleMatrix;
}

// If a simple quad defined as a -1 to 1 XY unit square is transformed to
// the camera view with the given modelView matrix, it can alternately be
// drawn as a TimeWarp overlay image to take advantage of the full window
// resolution, which is usually higher than the eye buffer textures, and
// avoid resampling both into the eye buffer, and again to the screen.
// This is used for high quality movie screens and user interface planes.
//
// Note that this is NOT an MVP matrix -- the "projection" is handled
// by the distortion process.
//
// The exact composition of the overlay image and the base image is
// determined by the warpProgram, you may still need to draw the geometry
// into the eye buffer to punch a hole in the alpha channel to let the
// overlay/underlay show through.
//
// This utility functions converts a model-view matrix that would normally
// draw a -1 to 1 unit square to the view into a TanAngle matrix for an
// overlay surface.
//
// The resulting z value should be straight ahead distance to the plane.
// The x and y values will be pre-multiplied by z for projective texturing.
inline dpnMatrix4 dpnutilMatrix4_TanAngleMatrixFromUnitSquare( const dpnMatrix4 * modelView )
{
	const dpnMatrix4 inv = dpnutilMatrix4_Inverse( modelView );
	dpnMatrix4 m;
	m.M[0][0] = 0.5f * inv.M[2][0] - 0.5f * ( inv.M[0][0] * inv.M[2][3] - inv.M[0][3] * inv.M[2][0] );
	m.M[0][1] = 0.5f * inv.M[2][1] - 0.5f * ( inv.M[0][1] * inv.M[2][3] - inv.M[0][3] * inv.M[2][1] );
	m.M[0][2] = 0.5f * inv.M[2][2] - 0.5f * ( inv.M[0][2] * inv.M[2][3] - inv.M[0][3] * inv.M[2][2] );
	m.M[0][3] = 0.0f;
	m.M[1][0] = 0.5f * inv.M[2][0] + 0.5f * ( inv.M[1][0] * inv.M[2][3] - inv.M[1][3] * inv.M[2][0] );
	m.M[1][1] = 0.5f * inv.M[2][1] + 0.5f * ( inv.M[1][1] * inv.M[2][3] - inv.M[1][3] * inv.M[2][1] );
	m.M[1][2] = 0.5f * inv.M[2][2] + 0.5f * ( inv.M[1][2] * inv.M[2][3] - inv.M[1][3] * inv.M[2][2] );
	m.M[1][3] = 0.0f;
	m.M[2][0] = m.M[3][0] = inv.M[2][0];
	m.M[2][1] = m.M[3][1] = inv.M[2][1];
	m.M[2][2] = m.M[3][2] = inv.M[2][2];
	m.M[2][3] = m.M[3][3] = 0.0f;
	return m;
}

// Utility function to calculate external velocity for smooth stick yaw turning.
// To reduce judder in FPS style experiences when the application framerate is
// lower than the vsync rate, the rotation from a joypad can be applied to the
// view space distorted eye vectors before applying the time warp.
inline dpnMatrix4 dpnutilMatrix4_CalculateExternalVelocity( const dpnMatrix4 * viewMatrix, const float yawRadiansPerSecond )
{
	const float angle = yawRadiansPerSecond * ( -1.0f / 60.0f );
	const float sinHalfAngle = sinf( angle * 0.5f );
	const float cosHalfAngle = cosf( angle * 0.5f );

	// Yaw is always going to be around the world Y axis
	dpnQuarterion quat;
	quat.x = viewMatrix->M[0][1] * sinHalfAngle;
	quat.y = viewMatrix->M[1][1] * sinHalfAngle;
	quat.z = viewMatrix->M[2][1] * sinHalfAngle;
	quat.w = cosHalfAngle;
	return dpnutilMatrix4_CreateFromQuaternion( &quat );
}

// Utility function to default initialize the dpnHmdParms.
static inline dpnHmdParms dpnutilDefaultHmdParms()
{
	dpnHmdParms parms;
	memset( &parms, 0, sizeof( parms ) );

	parms.ipd	= 0.0640f;
	parms.eyeHeight					= 1.6750f;
	parms.depth			= 0.0805f;
	parms.height			= 0.0750f;

	return parms;
}

//-----------------------------------------------------------------
// Eye view matrix helper functions.
//-----------------------------------------------------------------

// Utility function to get the center eye offset based on the head orientation using a head-on-a-stick model.
static inline dpnVector3 dpnutilGetHeadModelCenterEyeOffset(	const dpnHmdParms * headModelParms,
																const dpnMatrix4 * headOrientation )
{
	const dpnHmdParms * p = headModelParms;
	const dpnMatrix4 * m = headOrientation;
	const dpnVector3 headModelCenterEyeOffset =
	{
		m->M[0][1] * p->height - m->M[0][2] * p->depth,
		m->M[1][1] * p->height - m->M[1][2] * p->depth - p->height,
		m->M[2][1] * p->height - m->M[2][2] * p->depth
	};
	return headModelCenterEyeOffset;
}

// Utility function to get the center eye transform.
// Pass in NULL for 'input' if there is no additional controller input.
static inline dpnMatrix4 dpnutilGetCenterEyeTransform(	const dpnHmdParms * headModelParms,
														const dpnSensorData * tracking,
														const dpnMatrix4 * input )
{
	const bool positionTracked = ( tracking->status & dpnSensorStatus_PositionTracked ) != 0;
	const dpnMatrix4 centerEyeRotation = dpnutilMatrix4_CreateFromQuaternion( &tracking->pose.q );
	const dpnVector3 centerEyeOffset = positionTracked ? tracking->pose.p :
											dpnutilGetHeadModelCenterEyeOffset( headModelParms, &centerEyeRotation );
	const dpnMatrix4 centerEyeTranslation = dpnutilMatrix4_CreateTranslation( centerEyeOffset.x, centerEyeOffset.y, centerEyeOffset.z );
	const dpnMatrix4 centerEyeTransform = dpnutilMatrix4_Multiply( &centerEyeTranslation, &centerEyeRotation );
	return ( input == NULL ) ? centerEyeTransform : dpnutilMatrix4_Multiply( input, &centerEyeTransform );
}

// Utility function to get the center eye view matrix.
// Pass in NULL for 'input' if there is no additional controller input.
static inline dpnMatrix4 dpnutilGetCenterEyeViewMatrix(	const dpnHmdParms * headModelParms,
														const dpnSensorData * tracking,
														const dpnMatrix4 * input )
{
	const dpnMatrix4 centerEyeTransform = dpnutilGetCenterEyeTransform( headModelParms, tracking, input );
	return dpnutilMatrix4_Inverse( &centerEyeTransform );
}

// Utility function to get the eye view matrix based on the center eye view matrix and the IPD.
static inline dpnMatrix4 dpnutilGetEyeViewMatrix(	const dpnHmdParms * headModelParms,
													const dpnMatrix4 * centerEyeViewMatrix,
													const int eye )
{
	const float eyeOffset = ( eye ? -0.5f : 0.5f ) * headModelParms->ipd;
	const dpnMatrix4 eyeOffsetMatrix = dpnutilMatrix4_CreateTranslation( eyeOffset, 0.0f, 0.0f );
	return dpnutilMatrix4_Multiply( &eyeOffsetMatrix, centerEyeViewMatrix );
}

#endif	// DEEPOON_SDK_UTILS_H
