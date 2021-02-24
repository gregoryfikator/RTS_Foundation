#pragma once

#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <math.h>
#include "Helpers/MathHelperClass.h"

class PositionClass
{
public:
	PositionClass();
	~PositionClass();

	void SetFrameTime(float);

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveRight(bool);
	void MoveLeft(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);

	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_forwardMoveSpeed, m_backwardMoveSpeed, m_leftMoveSpeed, m_rightMoveSpeed, m_upwardMoveSpeed, m_downwardMoveSpeed;

	float m_leftTurnSpeed, m_rightTurnSpeed, m_lookUpSpeed, m_lookDownSpeed;
};

#endif