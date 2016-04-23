//
//  ScreenParams.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef ScreenParams_hpp
#define ScreenParams_hpp

class UIScreenIgnore
{
};

class ScreenParams
{
public:
    ScreenParams(UIScreenIgnore *screen);
    ScreenParams(ScreenParams *screenParams);

    int width();
    int height();

    float widthInMeters();
    float heightInMeters();

    void setBorderSizeInMeters(float screenBorderSize);
    float borderSizeInMeters();

    bool equals(ScreenParams *other);

private:
    UIScreenIgnore *_screen;
    float _scale;
    float _xMetersPerPixel;
    float _yMetersPerPixel;
    float _borderSizeMeters;

    float pixelsPerInch(UIScreenIgnore *screen);
};


#endif /* ScreenParams_hpp */
