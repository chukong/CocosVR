//
//  FieldOfView.cpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#include "FieldOfView.h"

FieldOfView::FieldOfView() :
_left(s_defaultViewAngle),
_right(s_defaultViewAngle),
_bottom(s_defaultViewAngle),
_top(s_defaultViewAngle)
{
}

FieldOfView::FieldOfView(float left, float right, float bottom, float top)
{
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
}

FieldOfView::FieldOfView(FieldOfView *other)
{
    _left = other->_left;
    _right = other->_right;
    _bottom = other->_bottom;
    _top = other->_top;
}

void FieldOfView::setLeft(float left)
{
    _left = left;
}

float FieldOfView::left()
{
    return _left;
}

void FieldOfView::setRight(float right)
{
    _right = right;
}

float FieldOfView::right()
{
    return _right;
}

void FieldOfView::setBottom(float bottom)
{
    _bottom = bottom;
}

float FieldOfView::bottom()
{
    return _bottom;
}

void FieldOfView::setTop(float top)
{
    _top = top;
}

float FieldOfView::top()
{
    return _top;
}

cocos2d::Mat4 FieldOfView::toPerspectiveMatrix(float near, float far)
{
    float left = -tanf(CC_DEGREES_TO_RADIANS(_left)) * near;
    float right = tanf(CC_DEGREES_TO_RADIANS(_right)) * near;
    float bottom = -tanf(CC_DEGREES_TO_RADIANS(_bottom)) * near;
    float top = tanf(CC_DEGREES_TO_RADIANS(_top)) * near;

    cocos2d::Mat4 frustrum;
    cocos2d::Mat4::createOrthographicOffCenter(left, right, bottom, top, near, far, &frustrum);
    return frustrum;
}

bool FieldOfView::equals(FieldOfView *other)
{
    if (other == nullptr)
    {
        return false;
    }
    else if (other == this)
    {
        return true;
    }
    return
    (_left == other->_left)
    && (_right == other->_right)
    && (_bottom == other->_bottom)
    && (_top == other->_top);
}
