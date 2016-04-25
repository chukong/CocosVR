#ifndef CbApi_Helpers_h
#define CbApi_Helpers_h

#include "math.h"		// for cosf(), sinf(), tanf()
#include "string.h"		// for memset()
#include "CbApi_Config.h"
#include "CbApi_Types.h"

#define CBAPI_PI		3.14159265358979323846f
#define CBAPI_ZNEAR		0.1f

#if defined( __GNUC__ )
#  define   CBAPI_UNUSED(a)   do {__typeof__ (&a) __attribute__ ((unused)) __tmp = &a; } while(0)
#else
#  define   CBAPI_UNUSED(a)   (a)
#endif

//-----------------------------------------------------------------
// Matrix helper functions.
//-----------------------------------------------------------------

// Use left-multiplication to accumulate transformations.
static inline cbMatrix4 cbMatrix4_Multiply( const cbMatrix4 * a, const cbMatrix4 * b )
{
	cbMatrix4 out;
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

// Returns the transpose of a 4x4 matrix.
static inline cbMatrix4 cbMatrix4_Transpose( const cbMatrix4 * a )
{
	cbMatrix4 out;
	out.M[0][0] = a->M[0][0]; out.M[0][1] = a->M[1][0]; out.M[0][2] = a->M[2][0]; out.M[0][3] = a->M[3][0];
	out.M[1][0] = a->M[0][1]; out.M[1][1] = a->M[1][1]; out.M[1][2] = a->M[2][1]; out.M[1][3] = a->M[3][1];
	out.M[2][0] = a->M[0][2]; out.M[2][1] = a->M[1][2]; out.M[2][2] = a->M[2][2]; out.M[2][3] = a->M[3][2];
	out.M[3][0] = a->M[0][3]; out.M[3][1] = a->M[1][3]; out.M[3][2] = a->M[2][3]; out.M[3][3] = a->M[3][3];
	return out;
}

// Returns a 3x3 minor of a 4x4 matrix.
static inline float cbMatrix4_Minor( const cbMatrix4 * m, int r0, int r1, int r2, int c0, int c1, int c2 )
{
	return	m->M[r0][c0] * ( m->M[r1][c1] * m->M[r2][c2] - m->M[r2][c1] * m->M[r1][c2] ) -
			m->M[r0][c1] * ( m->M[r1][c0] * m->M[r2][c2] - m->M[r2][c0] * m->M[r1][c2] ) +
			m->M[r0][c2] * ( m->M[r1][c0] * m->M[r2][c1] - m->M[r2][c0] * m->M[r1][c1] );
}
 
// Returns the inverse of a 4x4 matrix.
static inline cbMatrix4 cbMatrix4_Inverse( const cbMatrix4 * m )
{
	const float rcpDet = 1.0f / (	m->M[0][0] * cbMatrix4_Minor( m, 1, 2, 3, 1, 2, 3 ) -
									m->M[0][1] * cbMatrix4_Minor( m, 1, 2, 3, 0, 2, 3 ) +
									m->M[0][2] * cbMatrix4_Minor( m, 1, 2, 3, 0, 1, 3 ) -
									m->M[0][3] * cbMatrix4_Minor( m, 1, 2, 3, 0, 1, 2 ) );
	cbMatrix4 out;
	out.M[0][0] =  cbMatrix4_Minor( m, 1, 2, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][1] = -cbMatrix4_Minor( m, 0, 2, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][2] =  cbMatrix4_Minor( m, 0, 1, 3, 1, 2, 3 ) * rcpDet;
	out.M[0][3] = -cbMatrix4_Minor( m, 0, 1, 2, 1, 2, 3 ) * rcpDet;
	out.M[1][0] = -cbMatrix4_Minor( m, 1, 2, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][1] =  cbMatrix4_Minor( m, 0, 2, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][2] = -cbMatrix4_Minor( m, 0, 1, 3, 0, 2, 3 ) * rcpDet;
	out.M[1][3] =  cbMatrix4_Minor( m, 0, 1, 2, 0, 2, 3 ) * rcpDet;
	out.M[2][0] =  cbMatrix4_Minor( m, 1, 2, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][1] = -cbMatrix4_Minor( m, 0, 2, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][2] =  cbMatrix4_Minor( m, 0, 1, 3, 0, 1, 3 ) * rcpDet;
	out.M[2][3] = -cbMatrix4_Minor( m, 0, 1, 2, 0, 1, 3 ) * rcpDet;
	out.M[3][0] = -cbMatrix4_Minor( m, 1, 2, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][1] =  cbMatrix4_Minor( m, 0, 2, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][2] = -cbMatrix4_Minor( m, 0, 1, 3, 0, 1, 2 ) * rcpDet;
	out.M[3][3] =  cbMatrix4_Minor( m, 0, 1, 2, 0, 1, 2 ) * rcpDet;
	return out;
}

// Returns a 4x4 identity matrix.
static inline cbMatrix4 cbMatrix4_CreateIdentity()
{
	cbMatrix4 out;
	out.M[0][0] = 1.0f; out.M[0][1] = 0.0f; out.M[0][2] = 0.0f; out.M[0][3] = 0.0f;
	out.M[1][0] = 0.0f; out.M[1][1] = 1.0f; out.M[1][2] = 0.0f; out.M[1][3] = 0.0f;
	out.M[2][0] = 0.0f; out.M[2][1] = 0.0f; out.M[2][2] = 1.0f; out.M[2][3] = 0.0f;
	out.M[3][0] = 0.0f; out.M[3][1] = 0.0f; out.M[3][2] = 0.0f; out.M[3][3] = 1.0f;
	return out;
}

// Returns a 4x4 homogeneous translation matrix.
static inline cbMatrix4 cbMatrix4_CreateTranslation( const float x, const float y, const float z )
{
	cbMatrix4 out;
	out.M[0][0] = 1.0f; out.M[0][1] = 0.0f; out.M[0][2] = 0.0f; out.M[0][3] = x;
	out.M[1][0] = 0.0f; out.M[1][1] = 1.0f; out.M[1][2] = 0.0f; out.M[1][3] = y;
	out.M[2][0] = 0.0f; out.M[2][1] = 0.0f; out.M[2][2] = 1.0f; out.M[2][3] = z;
	out.M[3][0] = 0.0f; out.M[3][1] = 0.0f; out.M[3][2] = 0.0f; out.M[3][3] = 1.0f;
	return out;
}

// Returns a 4x4 homogeneous rotation matrix.
static inline cbMatrix4 cbMatrix4_CreateRotation( const float radiansX, const float radiansY, const float radiansZ )
{
	const float sinX = sinf( radiansX );
	const float cosX = cosf( radiansX );
	const cbMatrix4 rotationX =
	{ {
		{ 1,    0,     0, 0 },
		{ 0, cosX, -sinX, 0 },
		{ 0, sinX,  cosX, 0 },
		{ 0,    0,     0, 1 }
	} };
	const float sinY = sinf( radiansY );
	const float cosY = cosf( radiansY );
	const cbMatrix4 rotationY =
	{ {
		{  cosY, 0, sinY, 0 },
		{     0, 1,    0, 0 },
		{ -sinY, 0, cosY, 0 },
		{     0, 0,    0, 1 }
	} };
	const float sinZ = sinf( radiansZ );
	const float cosZ = cosf( radiansZ );
	const cbMatrix4 rotationZ =
	{ {
		{ cosZ, -sinZ, 0, 0 },
		{ sinZ,  cosZ, 0, 0 },
		{    0,     0, 1, 0 },
		{    0,     0, 0, 1 }
	} };
	const cbMatrix4 rotationXY = cbMatrix4_Multiply( &rotationY, &rotationX );
	return cbMatrix4_Multiply( &rotationZ, &rotationXY );
}

// Returns a projection matrix based on the specified dimensions.
// The far plane is placed at infinity if farZ <= nearZ.
// An infinite projection matrix is preferred for rasterization because, except for
// things *right* up against the near plane, it always provides better precision:
//		"Tightening the Precision of Perspective Rendering"
//		Paul Upchurch, Mathieu Desbrun
//		Journal of Graphics Tools, Volume 16, Issue 1, 2012
static inline cbMatrix4 cbMatrix4_CreateProjection( const float minX, const float maxX,
											float const minY, const float maxY, const float nearZ, const float farZ )
{
	const float width = maxX - minX;
	const float height = maxY - minY;
	const float offsetZ = nearZ;	// set to zero for a [0,1] clip space

	cbMatrix4 out;
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

// Returns a projection matrix based on the given FOV.
static inline cbMatrix4 cbMatrix4_CreateProjectionFov( const float fovDegreesX, const float fovDegreesY,
												const float offsetX, const float offsetY, const float nearZ, const float farZ )
{
	const float halfWidth = nearZ * tanf( fovDegreesX * ( CBAPI_PI / 180.0f * 0.5f ) );
	const float halfHeight = nearZ * tanf( fovDegreesY * ( CBAPI_PI / 180.0f * 0.5f ) );

	const float minX = offsetX - halfWidth;
	const float maxX = offsetX + halfWidth;

	const float minY = offsetY - halfHeight;
	const float maxY = offsetY + halfHeight;

	return cbMatrix4_CreateProjection( minX, maxX, minY, maxY, nearZ, farZ );
}

// Returns the 4x4 rotation matrix for the given quaternion.
static inline cbMatrix4 cbMatrix4_CreateFromQuaternion( const cbQuat * q )
{
	const float ww = q->w * q->w;
	const float xx = q->x * q->x;
	const float yy = q->y * q->y;
	const float zz = q->z * q->z;

	cbMatrix4 out;
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

// Convert a standard projection matrix into a TexCoordsFromTanAngles matrix for
// the primary time warp surface.
static inline cbMatrix4 cbMatrix4_TanAngleMatrixFromProjection( const cbMatrix4 * projection )
{
	/*
		A projection matrix goes from a view point to NDC, or -1 to 1 space.
		Scale and bias to convert that to a 0 to 1 space.

		const ovrMatrix3f m =
		{ {
			{ projection->M[0][0],                0.0f, projection->M[0][2] },
			{                0.0f, projection->M[1][1], projection->M[1][2] },
			{                0.0f,                0.0f,               -1.0f }
		} };
		// Note that there is no Y-flip because eye buffers have 0,0 = left-bottom.
		const ovrMatrix3f s = ovrMatrix3f_CreateScaling( 0.5f, 0.5f );
		const ovrMatrix3f t = ovrMatrix3f_CreateTranslation( 0.5f, 0.5f );
		const ovrMatrix3f r0 = ovrMatrix3f_Multiply( &s, &m );
		const ovrMatrix3f r1 = ovrMatrix3f_Multiply( &t, &r0 );
		return r1;

		clipZ = ( z * projection[2][2] + projection[2][3] ) / ( projection[3][2] * z )
		z = projection[2][3] / ( clipZ * projection[3][2] - projection[2][2] )
		z = ( projection[2][3] / projection[3][2] ) / ( clipZ - projection[2][2] / projection[3][2] )
	*/
	const cbMatrix4 tanAngleMatrix =
	{ {
		{ 0.5f * projection->M[0][0], 0.0f, 0.5f * projection->M[0][2] - 0.5f, 0.0f },
		{ 0.0f, 0.5f * projection->M[1][1], 0.5f * projection->M[1][2] - 0.5f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		// Store the values to convert a clip-Z to a linear depth in the unused matrix elements.
		{ projection->M[2][2], projection->M[2][3], projection->M[3][2], 1.0f }
	} };
	return tanAngleMatrix;
}

// If a simple quad defined as a -1 to 1 XY unit square is transformed to
// the camera view with the given modelView matrix, it can alternately be
// drawn as a time warp overlay image to take advantage of the full window
// resolution, which is usually higher than the eye buffer textures, and
// avoids resampling both into the eye buffer, and again to the screen.
// This is used for high quality movie screens and user interface planes.
//
// Note that this is NOT an MVP matrix -- the "projection" is handled
// by the distortion process.
//
// The exact composition of the overlay image and the base image is
// determined by the warp program, you may still need to draw the geometry
// into the eye buffer to punch a hole in the alpha channel to let the
// overlay/underlay show through.
//
// This utility functions converts a model-view matrix that would normally
// draw a -1 to 1 unit square to the view into a TexCoordsFromTanAngles matrix 
// for an overlay surface.
//
// The resulting z value should be straight ahead distance to the plane.
// The x and y values will be pre-multiplied by z for projective texturing.
static inline cbMatrix4 cbMatrix4_TanAngleMatrixFromUnitSquare( const cbMatrix4 * modelView )
{
	/*
		// Take the inverse of the view matrix because the view matrix transforms the unit square
		// from world space into view space, while the matrix needed here is the one that transforms
		// the unit square from view space to world space.
		const cbMatrix4 inv = cbMatrix4_Inverse( modelView );
		// This matrix calculates the projection onto the (-1, 1) X and Y axes of the unit square,
		// of the intersection of the vector (tanX, tanY, -1) with the plane described by the matrix
		// that transforms the unit square into world space.
		const ovrMatrix3f m =
		{ {
			{	inv.M[0][0] * inv.M[2][3] - inv.M[0][3] * inv.M[2][0],
				inv.M[0][1] * inv.M[2][3] - inv.M[0][3] * inv.M[2][1],
				inv.M[0][2] * inv.M[2][3] - inv.M[0][3] * inv.M[2][2] },
			{	inv.M[1][0] * inv.M[2][3] - inv.M[1][3] * inv.M[2][0],
				inv.M[1][1] * inv.M[2][3] - inv.M[1][3] * inv.M[2][1],
				inv.M[1][2] * inv.M[2][3] - inv.M[1][3] * inv.M[2][2] },
			{	- inv.M[2][0],
				- inv.M[2][1],
				- inv.M[2][2] }
		} };
		// Flip the Y because textures have 0,0 = left-top as opposed to left-bottom.
		const ovrMatrix3f f = ovrMatrix3f_CreateScaling( 1.0f, -1.0f );
		const ovrMatrix3f s = ovrMatrix3f_CreateScaling( 0.5f, 0.5f );
		const ovrMatrix3f t = ovrMatrix3f_CreateTranslation( 0.5f, 0.5f );
		const ovrMatrix3f r0 = ovrMatrix3f_Multiply( &f, &m );
		const ovrMatrix3f r1 = ovrMatrix3f_Multiply( &s, &r0 );
		const ovrMatrix3f r2 = ovrMatrix3f_Multiply( &t, &r1 );
		return r2;
	*/

	const cbMatrix4 inv = cbMatrix4_Inverse( modelView );

	cbMatrix4 m;
	m.M[0][0] = + 0.5f * ( inv.M[0][0] * inv.M[2][3] - inv.M[0][3] * inv.M[2][0] ) - 0.5f * inv.M[2][0];
	m.M[0][1] = + 0.5f * ( inv.M[0][1] * inv.M[2][3] - inv.M[0][3] * inv.M[2][1] ) - 0.5f * inv.M[2][1];
	m.M[0][2] = + 0.5f * ( inv.M[0][2] * inv.M[2][3] - inv.M[0][3] * inv.M[2][2] ) - 0.5f * inv.M[2][2];
	m.M[0][3] = 0.0f;
										 									
	m.M[1][0] = - 0.5f * ( inv.M[1][0] * inv.M[2][3] - inv.M[1][3] * inv.M[2][0] ) - 0.5f * inv.M[2][0];
	m.M[1][1] = - 0.5f * ( inv.M[1][1] * inv.M[2][3] - inv.M[1][3] * inv.M[2][1] ) - 0.5f * inv.M[2][1];
	m.M[1][2] = - 0.5f * ( inv.M[1][2] * inv.M[2][3] - inv.M[1][3] * inv.M[2][2] ) - 0.5f * inv.M[2][2];
	m.M[1][3] = 0.0f;

	m.M[2][0] = - inv.M[2][0];
	m.M[2][1] = - inv.M[2][1];
	m.M[2][2] = - inv.M[2][2];
	m.M[2][3] = 0.0f;

	m.M[3][0] = 0.0f;
	m.M[3][1] = 0.0f;
	m.M[3][2] = 0.0f;
	m.M[3][3] = 1.0f;
	return m;
}

// Utility function to calculate external velocity for smooth stick yaw turning.
// To reduce judder in FPS style experiences when the application framerate is
// lower than the vsync rate, the rotation from a joypad can be applied to the
// view space distorted eye vectors before applying the time warp.
static inline cbMatrix4 cbMatrix4_CalculateExternalVelocity( const cbMatrix4 * viewMatrix, const float yawRadiansPerSecond )
{
	const float angle = yawRadiansPerSecond * ( -1.0f / 60.0f );
	const float sinHalfAngle = sinf( angle * 0.5f );
	const float cosHalfAngle = cosf( angle * 0.5f );

	// Yaw is always going to be around the world Y axis
	cbQuat quat;
	quat.x = viewMatrix->M[0][1] * sinHalfAngle;
	quat.y = viewMatrix->M[1][1] * sinHalfAngle;
	quat.z = viewMatrix->M[2][1] * sinHalfAngle;
	quat.w = cosHalfAngle;
	return cbMatrix4_CreateFromQuaternion( &quat );
}

//-----------------------------------------------------------------
// Default initialization helper functions.
//-----------------------------------------------------------------

// Utility function to get the eye view matrix based on the center eye view matrix and the IPD.
static inline cbMatrix4 cbapi_GetEyeViewMatrix(	const cbDeviceParams * deviceParams,
													const cbMatrix4 * headViewMatrix,
													const int eye )
{
	const float eyeOffset = ( eye ? -0.5f : 0.5f ) * deviceParams->interLensDistance;
	const cbMatrix4 eyeOffsetMatrix = cbMatrix4_CreateTranslation( eyeOffset, 0.0f, 0.0f );
	return cbMatrix4_Multiply( &eyeOffsetMatrix, headViewMatrix );
}

#endif
