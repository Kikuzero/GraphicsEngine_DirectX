#pragma once
#include <vector>
#include <SimpleMath.h>
#include "../ECS/IComponent.h"

struct LinesComponent : IComponent
{
	std::vector<DirectX::SimpleMath::Vector3> linesPoints;
};
