//
//  Viewport.cpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/15/16.
//
//

#include "Viewport.h"

void Viewport::setViewport(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Viewport::setGLViewport()
{
    glViewport(x, y, width, height);
}

void Viewport::setGLScissor()
{
    glScissor(x, y, width, height);
}

cocos2d::Rect Viewport::toCGRect()
{
    return cocos2d::Rect(x, y, width, height);
}
