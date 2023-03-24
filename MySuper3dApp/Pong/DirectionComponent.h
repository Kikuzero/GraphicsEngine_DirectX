#pragma once
#include <DirectXMath.h>
#include "../ECS/IComponent.h"

struct DirectionComponent : IComponent
{
	DirectX::SimpleMath::Vector3 direction;
};
