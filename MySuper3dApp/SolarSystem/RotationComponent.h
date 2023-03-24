#pragma once
#include <SimpleMath.h>
#include "../ECS/IComponent.h"

struct RotationComponent : IComponent
{
	float yaw;
	float pitch;
};
