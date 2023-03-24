#pragma once
#include "IComponent.h"

struct Gravity : IComponent
{
	float force;
};

struct RigidBody : IComponent
{
	float velocity;
	float acceleration;
};

struct Transform : IComponent
{
	float position;
	float rotation;
	float scale;
};