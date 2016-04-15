//
//  CardboardDeviceParams.cpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#include "CardboardDeviceParams.h"

#include "Distortion.h"
#include "FieldOfView.h"


CardboardDeviceParams::CardboardDeviceParams() :
_vendor("com.google"),
_model("cardboard"),
_interLensDistance(0.06f),
_verticalDistanceToLensCenter(0.035f),
_screenToLensDistance(0.042f)
{
    _distortion = new Distortion();
    _maximumLeftEyeFOV = new FieldOfView();
}

CardboardDeviceParams::CardboardDeviceParams(CardboardDeviceParams* params)
{
    _vendor = params->_vendor;
    _model = params->_model;

    _interLensDistance = params->_interLensDistance;
    _verticalDistanceToLensCenter = params->_verticalDistanceToLensCenter;
    _screenToLensDistance = params->_screenToLensDistance;

    _maximumLeftEyeFOV = new FieldOfView(params->_maximumLeftEyeFOV);
    _distortion = new Distortion(params->_distortion);
}

CardboardDeviceParams::~CardboardDeviceParams()
{
    if (_distortion != nullptr) { delete _distortion; }
    if (_maximumLeftEyeFOV != nullptr) { delete _maximumLeftEyeFOV; }
}

const std::string& CardboardDeviceParams::vendor() const
{
    return _vendor;
}

const std::string& CardboardDeviceParams::model() const
{
    return _model;
}

float CardboardDeviceParams::interLensDistance()
{
    return _interLensDistance;
}

float CardboardDeviceParams::verticalDistanceToLensCenter()
{
    return _verticalDistanceToLensCenter;
}

float CardboardDeviceParams::screenToLensDistance()
{
    return _screenToLensDistance;
}

FieldOfView *CardboardDeviceParams::maximumLeftEyeFOV()
{
    return _maximumLeftEyeFOV;
}

Distortion *CardboardDeviceParams::distortion()
{
    return _distortion;
}

bool CardboardDeviceParams::equals(CardboardDeviceParams *other)
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
    (_vendor == other->_vendor)
    && (_model == other->_model)
    && (interLensDistance() == other->interLensDistance())
    && (verticalDistanceToLensCenter() == other->verticalDistanceToLensCenter())
    && (screenToLensDistance() == other->screenToLensDistance())
    && (maximumLeftEyeFOV()->equals(other->maximumLeftEyeFOV()))
    && (distortion()->equals(other->distortion()));
}
