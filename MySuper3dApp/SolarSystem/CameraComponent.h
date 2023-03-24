#pragma once
#include <SimpleMath.h>
#include "../ECS/IComponent.h"

struct CameraComponent : IComponent
{
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projMatrix;

	DirectX::SimpleMath::Matrix GetCameraMatrix() const
	{
		return viewMatrix * projMatrix;
	}

	DirectX::SimpleMath::Vector3 GetPoition() const
	{
		DirectX::SimpleMath::Matrix inv;
		viewMatrix.Invert(inv);
		return inv.Translation();
	}

	DirectX::SimpleMath::Vector3 GetForwardVector() const
	{
		DirectX::SimpleMath::Matrix inv;
		viewMatrix.Invert(inv);
		return inv.Forward();
	}
	DirectX::SimpleMath::Vector3 GetUpVector() const
	{
		DirectX::SimpleMath::Matrix inv;
		viewMatrix.Invert(inv);
		return inv.Up();
	}
};