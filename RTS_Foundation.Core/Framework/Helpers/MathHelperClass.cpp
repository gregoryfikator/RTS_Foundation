#include "MathHelperClass.h"

float MathHelperClass::DegreesToRadiansMultiplier = M_PI / 180.0f;

float MathHelperClass::DegreesToRadians(float degrees)
{
	return degrees * MathHelperClass::DegreesToRadiansMultiplier;
}