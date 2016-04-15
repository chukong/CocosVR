//
//  ScreenParams.cpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#include "ScreenParams.h"

#include <sys/utsname.h>
#include "cocos2d.h"


ScreenParams::ScreenParams(UIScreenIgnore *screen)
{
    _screen = screen;
    _scale = 2;

    float screenPixelsPerInch = pixelsPerInch(screen);

    const float metersPerInch = 0.0254f;
    const float defaultBorderSizeMeters = 0.003f;
    _xMetersPerPixel = (metersPerInch / screenPixelsPerInch);
    _yMetersPerPixel = (metersPerInch / screenPixelsPerInch);

    _borderSizeMeters = defaultBorderSizeMeters;
}

ScreenParams::ScreenParams(ScreenParams *screenParams)
{
    _scale = screenParams->_scale;
    _xMetersPerPixel = screenParams->_xMetersPerPixel;
    _yMetersPerPixel = screenParams->_yMetersPerPixel;
    _borderSizeMeters = screenParams->_borderSizeMeters;
}

int ScreenParams::width()
{
    return cocos2d::Director::getInstance()->getWinSize().width;
}

int ScreenParams::height()
{
    return cocos2d::Director::getInstance()->getWinSize().height;
}

float ScreenParams::widthInMeters()
{
    float meters = width() * _xMetersPerPixel;
    return meters;
}

float ScreenParams::heightInMeters()
{
    float meters = height() * _yMetersPerPixel;
    return meters;
}

void ScreenParams::setBorderSizeInMeters(float screenBorderSize)
{
    _borderSizeMeters = screenBorderSize;
}

float ScreenParams::borderSizeInMeters()
{
    return _borderSizeMeters;
}

bool ScreenParams::equals(ScreenParams *other)
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
    (width() == other->width())
    && (height() == other->height())
    && (widthInMeters() == other->widthInMeters())
    && (heightInMeters() == other->heightInMeters())
    && (borderSizeInMeters() == other->borderSizeInMeters());
}

float ScreenParams::pixelsPerInch(UIScreenIgnore *screen)
{
    // Default iPhone retina pixels per inch
    float pixelsPerInch = 163.0f * 2;
    return pixelsPerInch;
}
