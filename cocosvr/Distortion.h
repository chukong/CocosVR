//
//  Distortion.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef Distortion_hpp
#define Distortion_hpp

class Distortion
{
public:
    Distortion();
    Distortion(Distortion *other);

    void setCoefficients(float *coefficients);
    float *coefficients();

    float distortionFactor(float radius);
    float distort(float radius);
    float distortInverse(float radius);
    bool equals(Distortion *other);

private:
    constexpr static int s_numberOfCoefficients = 2;
    float _coefficients[s_numberOfCoefficients];
};

#endif /* Distortion_hpp */
