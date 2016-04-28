//
//  FieldOfView.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef FieldOfView_hpp
#define FieldOfView_hpp

#include "math/Mat4.h"

class FieldOfView
{
public:
    FieldOfView();
    FieldOfView(float left, float right, float bottom, float top);
    FieldOfView(FieldOfView *other);

    void setLeft(float left);
    float left();

    void setRight(float right);
    float right();

    void setBottom(float bottom);
    float bottom();

    void setTop(float top);
    float top();

    cocos2d::Mat4 toPerspectiveMatrix(float near, float far);

    bool equals(FieldOfView *other);

private:
    constexpr static float s_defaultViewAngle = 40.0f;

    float _left;
    float _right;
    float _bottom;
    float _top;
};

#endif /* FieldOfView_hpp */
