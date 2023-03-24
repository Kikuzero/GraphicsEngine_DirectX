#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "../MySuper3dApp/TransitionConstBuffer.h"

class RenderSystem : public System
{
public:
	RenderSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			const auto ge = engine->GetComponent<RenderComponent>(entity);
			const auto transform = engine->GetComponent<TransformComponent>(entity);

			auto tr = TransitionConstBuffer{ .x = transform.localPosition.x, .y = transform.localPosition.y, ._0 = -1, ._1 = -1 };

			if (ge.cb == nullptr)
			{
				ge.graphicsEngine->Render(ge.vb->GetBuffer(), nullptr, ge.ib->GetBuffer(), ge.ib->GetSize());
				continue;
			}

			ge.graphicsEngine->UpdateConstantBuffer<TransitionConstBuffer>(ge.cb->GetBuffer(), tr);
			ge.graphicsEngine->Render(ge.vb->GetBuffer(), ge.cb->GetBuffer(), ge.ib->GetBuffer(), ge.ib->GetSize());
		}
	}
};