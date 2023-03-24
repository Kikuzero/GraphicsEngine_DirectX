#pragma once
#include "../ECS/IComponent.h"
#include "DirectionLightData.h"

struct DirectionalLightComponent : IComponent
{
	DirectionLightData directionLightData;
};