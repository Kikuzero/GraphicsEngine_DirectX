#pragma once
#include "../ECS/IComponent.h"

enum class SideUploader
{
	None,
	Left,
	Right
};

struct CountUploadedComponent : IComponent
{
	SideUploader side;
	bool uploaded;
};