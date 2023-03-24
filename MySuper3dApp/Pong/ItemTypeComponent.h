#pragma once
#include "../ECS/IComponent.h"

enum class ItemType
{
	Ball,
	Player,
	Wall,
	Goal
};

struct ItemTypeComponent : IComponent
{
	ItemType type;
};