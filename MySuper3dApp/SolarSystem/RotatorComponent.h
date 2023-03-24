#pragma once
#include <SimpleMath.h>
#include "../ECS/IComponent.h"

struct RotatorComponent : IComponent
{
	DirectX::SimpleMath::Vector3 vector;
	float rotationSpeed;
};
