//
//  Eye.cpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/19/16.
//
//

#include "Eye.h"

#include "FieldOfView.h"
#include "Viewport.h"


Eye::Eye(Type eyeType) :
_type(eyeType),
_eyeView(cocos2d::Mat4::IDENTITY),
_projectionChanged(true),
_perspective(cocos2d::Mat4::IDENTITY),
_lastZNear(0),
_lastZFar(0)
{
    _viewport = new Viewport();
    _fov = new FieldOfView();
}

Eye::~Eye()
{
    if (_viewport != nullptr) { delete _viewport; }
    if (_fov != nullptr) { delete _fov; }
}

Eye::Type Eye::type()
{
    return _type;
}

cocos2d::Mat4 Eye::eyeView()
{
    return _eyeView;
}

void Eye::setEyeView(const cocos2d::Mat4& eyeView)
{
    _eyeView = eyeView;
}

cocos2d::Mat4 Eye::perspective(float zNear, float zFar)
{
    if (!_projectionChanged && _lastZNear == zNear && _lastZFar == zFar)
    {
        return _perspective;
    }
    _perspective = fov()->toPerspectiveMatrix(zNear, zFar);
    _lastZNear = zNear;
    _lastZFar = zFar;
    _projectionChanged = false;
    return _perspective;
}

Viewport *Eye::viewport()
{
    return _viewport;
}

FieldOfView *Eye::fov()
{
    return _fov;
}

void Eye::setProjectionChanged()
{
    _projectionChanged = true;
}
