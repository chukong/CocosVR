//
//  DistortionMesh.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef DistortionMesh_hpp
#define DistortionMesh_hpp

class Distortion;

class DistortionMesh
{
public:
    int _indices;
    int _arrayBufferID;
    int _elementBufferID;

    DistortionMesh();
    DistortionMesh(Distortion *distortionRed,
                   Distortion *distortionGreen,
                   Distortion *distortionBlue,
                   float screenWidth, float screenHeight,
                   float xEyeOffsetScreen, float yEyeOffsetScreen,
                   float textureWidth, float textureHeight,
                   float xEyeOffsetTexture, float yEyeOffsetTexture,
                   float viewportXTexture, float viewportYTexture,
                   float viewportWidthTexture,
                   float viewportHeightTexture,
                   bool vignetteEnabled);
};

#endif /* DistortionMesh_hpp */
