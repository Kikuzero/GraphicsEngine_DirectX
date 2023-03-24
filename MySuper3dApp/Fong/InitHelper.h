#pragma once
#include "../ECS/Engine.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "../MySuper3dApp/ModelManager.h"
#include "../Pong/RenderComponent.h"
#include "../Pong/TransformComponent.h"
#include "../Pong/PreRenderSystem.h"
#include "../Pong/AfterRenderSystem.h"
#include "../Pong/DirectionComponent.h"
#include "../SolarSystem/CameraComponent.h"
#include "../SolarSystem/RotatorComponent.h"
#include "../SolarSystem/RotationSpeedComponent.h"
#include "../SolarSystem/CameraControllerSystem.h"
#include "../SolarSystem/ProjectionRendererSystem.h"
#include "../SolarSystem/SolarSystem.h"
#include "../SolarSystem/IsObjectComponent.h"
#include "../SolarSystem/RotationComponent.h"
#include "DirectionalLightComponent.h"
#include "LinesRenderer.h"
#include "IsLinesComponent.h"

void InitComponents(Engine& _engine)
{
	_engine.RegisterComponent<
		RenderComponent,
		TransformComponent,
		DirectionComponent,
		CameraComponent,
		RotationSpeedComponent,
		SpeedComponent,
		RotatorComponent,
		IsObjectComponent,
		RotationComponent,
		DirectionalLightComponent,
		IsLinesComponent
	>();
}

void InitSystems(Engine& _engine)
{
	_engine.RegisterSystem<CameraControllerSystem>()
		->AddComponentsToSystemNode< CameraControllerSystem,
		TransformComponent,
		DirectionComponent,
		CameraComponent,
		SpeedComponent,
		RotationSpeedComponent,
		RotationComponent
		>();

	_engine.RegisterSystem<SolarSystem>()
		->AddComponentsToSystemNode< SolarSystem,
		TransformComponent,
		RotatorComponent,
		IsObjectComponent
		>();

	_engine.RegisterSystem<PreRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< PreRenderSystem,
		RenderComponent
		>();

	_engine.RegisterSystem<ProjectionRendererSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< ProjectionRendererSystem,
		RenderComponent,
		TransformComponent,
		IsObjectComponent
		>();

	_engine.RegisterSystem<LinesRenderer>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< LinesRenderer,
		RenderComponent,
		TransformComponent,
		IsLinesComponent
		>();

	_engine.RegisterSystem<AfterRenderSystem>(SystemTypes::RuntimeUnfixed)
		->AddComponentsToSystemNode< AfterRenderSystem,
		RenderComponent
		>();
}

Entity CreateApple(
	Engine& _engine,
	GraphicsEngine& _ge,
	DirectX::SimpleMath::Vector3 _position = DirectX::SimpleMath::Vector3::Zero,
	DirectX::SimpleMath::Vector3 _scale = DirectX::SimpleMath::Vector3::One,
	DirectX::SimpleMath::Vector3 _rotationVector = DirectX::SimpleMath::Vector3::Zero,
	DirectX::SimpleMath::Vector3 _color = DirectX::SimpleMath::Vector3::One,
	float _rotationSpeed = 0,
	Entity _parent = -1)
{
	ModelManager mm;
	if (!mm.Load("../Objects/Apple/Apple.obj", _color)) return -1;

	auto verteces = mm.GetMeshes();

	DirectX::SimpleMath::Vector3 rotVector = DirectX::SimpleMath::Vector3{ 0.f, 1.f, 0.f };
	float rotSpeed = 300.f;

	Entity apple = _engine.CreateEntity(_engine.EntityNameCreater("Apple"));
	_engine.AddComponents(apple,
		RenderComponent(&_ge, verteces.at(0).vertices, verteces.at(0).indices, Topology::Triangles),
		TransformComponent{
			.localPosition = _position,
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = _scale,
			.parent = _parent
		},
		RotatorComponent{ .vector = _rotationVector, .rotationSpeed = _rotationSpeed },
		IsObjectComponent{}
	);

	return apple;
}

Entity CreateCamera(Engine& _engine, GraphicsEngine& _ge)
{
	auto cameraMovingSpeed = 10.f;
	auto cameraRotationSpeed = 10.f;

	auto sizes = _ge.GetWindowSizes();

	Entity camera = _engine.CreateEntity(_engine.EntityNameCreater("Camera"));
	_engine.AddComponents(camera,
		SpeedComponent(cameraMovingSpeed),
		RotationSpeedComponent{ .currentSpeed = cameraRotationSpeed, .defaultSpeed = cameraRotationSpeed },
		DirectionComponent{ .direction = DirectX::SimpleMath::Vector3{ 0.f,0.f,0.f } },
		CameraComponent{
			.viewMatrix = DirectX::SimpleMath::Matrix::Identity,
			.projMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
				(float)DirectX::XM_PI * 0.35f,
				(float)sizes.width / sizes.height,
				0.1f,
				10000.f)
		},
		RotationComponent{ .yaw = 0, .pitch = 0 },
		TransformComponent{
			.localPosition = DirectX::SimpleMath::Vector3{ 2.f,0.f,2.f },
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = DirectX::SimpleMath::Vector3{ 0.f,0.f,0.f } }
	);

	return camera;
}

Entity CreateDirectionalLight(Engine& _engine, GraphicsEngine& _ge, DirectX::SimpleMath::Vector3 _direction, DirectX::SimpleMath::Vector3 _color)
{
	auto sizes = _ge.GetWindowSizes();

	Entity light = _engine.CreateEntity(_engine.EntityNameCreater("DirectionalLight"));
	_engine.AddComponents(light,
		DirectionalLightComponent{
			.directionLightData = {
				DirectX::SimpleMath::Vector4::Zero,
				_direction,
				0,
				_color,
				0
			}
		}
	);

	return light;
}

//Entity CreatePointLight(Engine& _engine, GraphicsEngine& _ge, DirectX::SimpleMath::Vector3 _position, DirectX::SimpleMath::Vector3 _color)
//{
//	auto sizes = _ge.GetWindowSizes();
//
//	Entity light = _engine.CreateEntity(_engine.EntityNameCreater("DirectionalLight"));
//	_engine.AddComponents(light,
//		Poi{
//			.directionLightData = {
//				DirectX::SimpleMath::Vector4::Zero,
//				_position,
//				0,
//				_color,
//				0
//			}
//		}
//	);
//
//	return light;
//}


Entity CreatePlane(Engine& _engine, GraphicsEngine& _ge, DirectX::SimpleMath::Vector3 _color = DirectX::SimpleMath::Vector3::One)
{
	ModelManager mm;
	if (!mm.Load("../Objects/Plane/model.obj", _color)) return -1;

	auto verteces = mm.GetMeshes();

	Entity plane = _engine.CreateEntity(_engine.EntityNameCreater("Plane"));
	_engine.AddComponents(plane,
		RenderComponent(&_ge, verteces.at(0).vertices, verteces.at(0).indices, Topology::Lines),
		TransformComponent{
			.localPosition = DirectX::SimpleMath::Vector3{ 0.f, 0.f, 0.f },
			.localRotation = DirectX::SimpleMath::Quaternion(),
			.localScale = DirectX::SimpleMath::Vector3{ 1.f, 1.f, 1.f }
		},
		IsLinesComponent{}
	);

	return plane;
}