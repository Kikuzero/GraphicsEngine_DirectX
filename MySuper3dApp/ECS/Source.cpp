#include "Engine.h"
#include "TestComponents.h"
#include "TestSystem.h"

Engine* engine = new Engine();

int main()
{
	engine->Init();
	
	engine->RegisterComponent<Gravity, RigidBody, Transform>();

	engine->RegisterSystem<FirstSystem>(SystemTypes::Begin)
		->AddComponentsToSystemNode<FirstSystem, Transform>();

	engine->RegisterSystem<PhysicsSystem>()
		->AddComponentsToSystemNode<PhysicsSystem, Gravity, RigidBody, Transform>();

	engine->RegisterSystem<PrintSystem>(SystemTypes::RuntimeLate)
		->AddComponentsToSystemNode<PrintSystem, Gravity, RigidBody, Transform>();

	engine->RegisterSystem<EndSystem>(SystemTypes::End)
		->AddComponentsToSystemNode<EndSystem, Transform>();

	Entity entity = engine->CreateEntity();
	engine->AddComponents(entity, 
		Gravity{ .force = 5 },
		RigidBody{ .velocity = 0.0f, .acceleration = 0.0f }, 
		Transform{ .position = 0, .rotation = 0, .scale = 1 }
	);

	engine->Run();
}