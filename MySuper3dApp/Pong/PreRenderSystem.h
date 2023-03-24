#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "RenderComponent.h"

class PreRenderSystem : public System
{
public:
	PreRenderSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		if (mEntities.size() == 0) return;

		auto const& entity = *mEntities.begin();

		auto& ge = engine->GetComponent<RenderComponent>(entity);

		ge.graphicsEngine->PreRender();
	}
};