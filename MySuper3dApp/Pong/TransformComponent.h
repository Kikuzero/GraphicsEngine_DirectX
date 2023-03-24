#pragma once
#include <directxmath.h>
#include <SimpleMath.h>
#include "../ECS/IComponent.h"

struct TransformComponent: IComponent
{
	DirectX::SimpleMath::Vector3 localPosition;
	DirectX::SimpleMath::Quaternion localRotation;
	DirectX::SimpleMath::Vector3 localScale;

	Entity parent = (Entity) -1;

	DirectX::SimpleMath::Matrix GetModelMatrix(Engine* _engine)
	{
		DirectX::SimpleMath::Matrix model = DirectX::SimpleMath::Matrix::Identity;

		model *= DirectX::SimpleMath::Matrix::CreateScale(localScale);
		model *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(localRotation);
		model *= DirectX::SimpleMath::Matrix::CreateTranslation(localPosition);

		if (parent != -1)
			model *= _engine->GetComponent<TransformComponent>(parent).GetModelMatrix(_engine);

		return model;
	}

	DirectX::SimpleMath::Quaternion GetRotation(Engine* _engine) {
		return parent != -1
			? localRotation * _engine->GetComponent<TransformComponent>(parent).GetRotation(_engine)
			: localRotation;
	}

	void SetRotation(Engine* _engine, DirectX::SimpleMath::Quaternion rotation) {
		if (parent != -1) {
			DirectX::SimpleMath::Quaternion quat;
			_engine->GetComponent<TransformComponent>(parent).GetRotation(_engine).Inverse(quat);
			localRotation = quat * rotation;
		}
		else localRotation = rotation;
	}
};
