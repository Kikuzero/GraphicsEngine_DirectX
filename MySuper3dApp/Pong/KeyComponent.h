#pragma once
#include <set>
#include "../ECS/IComponent.h"

struct KeyComponent : IComponent
{
	std::set<Keys> pressedKeys;
};