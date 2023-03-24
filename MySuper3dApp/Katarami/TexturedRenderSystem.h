#pragma once
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "../Pong/RenderComponent.h"
#include "../Pong/TransformComponent.h"
#include "../SolarSystem/CameraComponent.h"
#include "TextureComponent.h"

class TexturedRenderSystem : public System
{
public:
	TexturedRenderSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			const auto ge = engine->GetComponent<RenderComponent>(entity);
			auto transform = engine->GetComponent<TransformComponent>(entity);
			auto texture = engine->GetComponent<TextureComponent>(entity);

			auto& graphicsEngine = ge.graphicsEngine;
			texture.sampler->SetSampler(graphicsEngine, 0);

			const auto camera = engine->FindEntityByName("Camera");
			const auto cameraComponent = engine->GetComponent<CameraComponent>(camera);

			graphicsEngine->SetupDrawTopology(ge.topology);
			graphicsEngine->SetupIndexBuffer(ge.ib);
			graphicsEngine->SetupVertexBuffer(ge.vb);

			auto mvp = transform.GetModelMatrix(engine) * cameraComponent.viewMatrix * cameraComponent.projMatrix;
			graphicsEngine->UpdateConstantBuffer<DirectX::SimpleMath::Matrix>(ge.cb->GetBuffer(), mvp.Transpose());
			graphicsEngine->SetupConstBuffer(ge.cb);

			texture.texture->SetTexture(graphicsEngine, 0);
			graphicsEngine->Draw();
		}
	}
};