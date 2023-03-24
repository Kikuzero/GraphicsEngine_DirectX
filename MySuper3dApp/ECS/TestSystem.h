#pragma once
#include <iostream>
#include "System.h"
#include "Engine.h"
#include "TestComponents.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem(Engine* _engine): System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& rigidBody = engine->GetComponent<RigidBody>(entity);
			auto& transform = engine->GetComponent<Transform>(entity);
			auto const& gravity = engine->GetComponent<Gravity>(entity);

			transform.position += rigidBody.velocity;

			rigidBody.velocity += gravity.force;
		}
	}
};


class PrintSystem : public System
{
public:
	PrintSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& rigidBody = engine->GetComponent<RigidBody>(entity);
			auto& transform = engine->GetComponent<Transform>(entity);
			auto const& gravity = engine->GetComponent<Gravity>(entity);

			std::cout << "transform: " << transform.position << "|"	<< transform.rotation << "\n";
		}
	}
};

class FirstSystem : public System
{
public:
	FirstSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& transform = engine->GetComponent<Transform>(entity);

			std::cout << "Init transform: " << transform.position << "|" << transform.rotation << "\n";
		}
	}
};

class EndSystem : public System
{
public:
	EndSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			auto& transform = engine->GetComponent<Transform>(entity);

			std::cout << "End transform: " << transform.position << "|" << transform.rotation << "\n";
		}
	}
};