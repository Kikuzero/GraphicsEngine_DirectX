#pragma once
#include "../ECS/IComponent.h"

struct SpeedComponent : IComponent
{
private:
	float defaultSpeed;
	float currentSpeed;

public:
	SpeedComponent(float _speed = 0)
	{
		defaultSpeed = _speed;
		currentSpeed = _speed;
	}

	void Up(float _val)
	{
		currentSpeed += _val;
	}

	void Down(float _val)
	{
		currentSpeed -= _val;

		if (currentSpeed < 0) currentSpeed = 0;
	}

	void ToDefault()
	{
		currentSpeed = defaultSpeed;
	}

	float Get()
	{
		return currentSpeed;
	}
};