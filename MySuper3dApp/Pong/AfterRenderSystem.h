#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "RenderComponent.h"

class AfterRenderSystem : public System
{
public:
	AfterRenderSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		if (mEntities.size() == 0) return;

		auto const& entity = *mEntities.begin();

		auto& ge = engine->GetComponent<RenderComponent>(entity);

		ge.graphicsEngine->AfterRender();
	}
};