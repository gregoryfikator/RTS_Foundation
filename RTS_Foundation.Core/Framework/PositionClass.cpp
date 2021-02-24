#include "PositionClass.h"

PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardMoveSpeed = 0.0f;
	m_backwardMoveSpeed = 0.0f;
	m_upwardMoveSpeed = 0.0f;
	m_downwardMoveSpeed = 0.0f;
	m_leftMoveSpeed = 0.0f;
	m_rightMoveSpeed = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed = 0.0f;
	m_lookDownSpeed = 0.0f;
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void PositionClass::MoveForward(bool keydown)
{
	float radians;

	if (keydown)
	{
		m_forwardMoveSpeed += m_frameTime * 0.001f;

		if (m_forwardMoveSpeed > (m_frameTime * 0.02f))
		{
			m_forwardMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_forwardMoveSpeed -= m_frameTime * 0.0007f;

		if (m_forwardMoveSpeed < 0.0f)
		{
			m_forwardMoveSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = MathHelperClass::DegreesToRadians(m_rotationY);

	// Update the position.
	m_positionX += sinf(radians) * m_forwardMoveSpeed;
	m_positionZ += cosf(radians) * m_forwardMoveSpeed;

	return;
}

void PositionClass::MoveBackward(bool keydown)
{
	float radians;

	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_backwardMoveSpeed += m_frameTime * 0.001f;

		if (m_backwardMoveSpeed > (m_frameTime * 0.02f))
		{
			m_backwardMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_backwardMoveSpeed -= m_frameTime * 0.0007f;

		if (m_backwardMoveSpeed < 0.0f)
		{
			m_backwardMoveSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	//radians = m_rotationY * 0.0174532925f;
	radians = MathHelperClass::DegreesToRadians(m_rotationY);

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardMoveSpeed;
	m_positionZ -= cosf(radians) * m_backwardMoveSpeed;

	return;
}

void PositionClass::MoveRight(bool keydown)
{
	float radians;

	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_rightMoveSpeed += m_frameTime * 0.001f;

		if (m_rightMoveSpeed > (m_frameTime * 0.02f))
		{
			m_rightMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_rightMoveSpeed -= m_frameTime * 0.0007f;

		if (m_rightMoveSpeed < 0.0f)
		{
			m_rightMoveSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = MathHelperClass::DegreesToRadians(m_rotationY);

	// Update the position.
	m_positionX += cosf(radians) * m_rightMoveSpeed;
	m_positionZ -= sinf(radians) * m_rightMoveSpeed;

	return;
}

void PositionClass::MoveLeft(bool keydown)
{
	float radians;

	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_leftMoveSpeed += m_frameTime * 0.001f;

		if (m_leftMoveSpeed > (m_frameTime * 0.02f))
		{
			m_leftMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_leftMoveSpeed -= m_frameTime * 0.0007f;

		if (m_leftMoveSpeed < 0.0f)
		{
			m_leftMoveSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = MathHelperClass::DegreesToRadians(m_rotationY);

	// Update the position.
	m_positionX -= cosf(radians) * m_leftMoveSpeed;
	m_positionZ += sinf(radians) * m_leftMoveSpeed;

	return;
}

void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_upwardMoveSpeed += m_frameTime * 0.003f;

		if (m_upwardMoveSpeed > (m_frameTime * 0.02f))
		{
			m_upwardMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_upwardMoveSpeed -= m_frameTime * 0.002f;

		if (m_upwardMoveSpeed < 0.0f)
		{
			m_upwardMoveSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardMoveSpeed;

	return;
}

void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_downwardMoveSpeed += m_frameTime * 0.003f;

		if (m_downwardMoveSpeed > (m_frameTime * 0.02f))
		{
			m_downwardMoveSpeed = m_frameTime * 0.02f;
		}
	}
	else
	{
		m_downwardMoveSpeed -= m_frameTime * 0.002f;

		if (m_downwardMoveSpeed < 0.0f)
		{
			m_downwardMoveSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardMoveSpeed;

	return;
}

void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY -= m_leftTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}

void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if (m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime * 0.005f;

		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}

void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if (m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime * 0.005f;

		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}