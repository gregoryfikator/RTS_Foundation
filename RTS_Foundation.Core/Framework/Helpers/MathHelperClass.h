#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef _MATHHELPERCLASS_H_
#define _MATHHELPERCLASS_H_

class MathHelperClass
{
public:
	static float DegreesToRadians(float degrees);

	static float DegreesToRadiansMultiplier;

private:
	MathHelperClass() {}
};

#endif