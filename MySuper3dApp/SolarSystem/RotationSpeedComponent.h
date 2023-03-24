#pragma once
#include "../ECS/IComponent.h"

struct RotationSpeedComponent : IComponent
{
	float currentSpeed;
	float defaultSpeed;
};