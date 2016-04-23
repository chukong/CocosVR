//
//  Viewport.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/15/16.
//
//

#ifndef Viewport_hpp
#define Viewport_hpp

#include "math/CCGeometry.h"

struct Viewport
{
public:
    int x;
    int y;
    int width;
    int height;

    void setViewport(int x, int y, int width, int height);

    void setGLViewport();
    void setGLScissor();

    cocos2d::Rect toCGRect();
};

#endif /* Viewport_hpp */
