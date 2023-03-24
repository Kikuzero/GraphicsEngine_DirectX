#pragma once
#include "../ECS/IComponent.h"

enum class FormType
{
	Circle,
	Quadrangle,
};

struct PhysicsComponent : IComponent
{
	FormType type;

	// For Quadrangle 
	float width, height;

	// For circle
	float radius;

	DirectX::XMFLOAT3 normalVector;
};