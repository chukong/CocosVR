//
//  CardboardDeviceParams.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef CardboardDeviceParams_hpp
#define CardboardDeviceParams_hpp

#include <string>

class Distortion;
class FieldOfView;

class CardboardDeviceParams
{
public:
    CardboardDeviceParams();
    CardboardDeviceParams(CardboardDeviceParams* params);
    ~CardboardDeviceParams();

    const std::string& vendor() const;
    const std::string& model() const;

    float interLensDistance();

    float verticalDistanceToLensCenter();

    float screenToLensDistance();

    FieldOfView *maximumLeftEyeFOV();
    Distortion *distortion();

    bool equals(CardboardDeviceParams *other);

private:
    std::string _vendor;
    std::string _model;
    std::string _version;
    float _interLensDistance;
    float _verticalDistanceToLensCenter;
    float _screenToLensDistance;

    FieldOfView *_maximumLeftEyeFOV;
    Distortion *_distortion;
};

#endif /* CardboardDeviceParams_hpp */
