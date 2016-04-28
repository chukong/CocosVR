//
//  Eye.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/19/16.
//
//

#ifndef Eye_hpp
#define Eye_hpp

#include "math/Mat4.h"

class FieldOfView;
class Viewport;


class Eye
{
public:

    typedef enum
    {
        TypeMonocular = 0,
        TypeLeft = 1,
        TypeRight = 2
    } Type;

    Eye(Type eye);
    ~Eye();

    Type type();

    cocos2d::Mat4 eyeView();
    void setEyeView(const cocos2d::Mat4& eyeView);
    cocos2d::Mat4 perspective(float zNear, float zFar);

    Viewport *viewport();
    FieldOfView *fov();

    void setProjectionChanged();

private:
    Type _type;
    cocos2d::Mat4 _eyeView;
    Viewport *_viewport;
    FieldOfView *_fov;
    bool _projectionChanged;
    cocos2d::Mat4 _perspective;
    float _lastZNear;
    float _lastZFar;
};

#endif /* Eye_hpp */
