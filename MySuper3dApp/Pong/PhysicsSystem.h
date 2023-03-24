#pragma once
#include <cmath>
#include "../ECS/Engine.h"
#include "../ECS/System.h"
#include "CustomVectorMath.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "DirectionComponent.h"
#include "ItemTypeComponent.h"
#include "CountUploadedComponent.h"
#include "SpeedComponent.h"

class PhysicsSystem : public System
{
private:
	enum class CorrectionType {
		None,
		Upper,
		Lower
	};

private:
	bool HasIntersection(float _cx, float _cy, float _cr, float _qx, float _qy, float _qwidth, float _qheight)
	{
		const auto circleDistanceX = abs(_cx - _qx);
		const auto circleDistanceY = abs(_cy - _qy);

		if (circleDistanceX > (_qwidth / 2 + _cr)) { return false; }
		if (circleDistanceY > (_qheight / 2 + _cr)) { return false; }

		if (circleDistanceX <= (_qwidth / 2)) { return true; }
		if (circleDistanceY <= (_qheight / 2)) { return true; }

		const auto cornerDistanceSq = pow((circleDistanceX - _qwidth / 2), 2) + pow((circleDistanceY - _qheight / 2), 2);

		return cornerDistanceSq <= (pow(_cr, 2));
	};

	bool HasIntersection(float _x1, float _y1, float _width1, float _height1, float _x2, float _y2, float _width2, float _height2)
	{
		return abs(_x1 - _x2) * 2 < (_width1 + _width2) && abs(_y1 - _y2) * 2 < (_height1 + _height2);
	}

	DirectX::XMFLOAT3 SetupNewChangeDirection(DirectX::XMFLOAT3 _oldDirection, DirectX::XMFLOAT3 _normalVector, CorrectionType _type = CorrectionType::None)
	{
		auto length = VectorLength(_oldDirection);
		auto lenVector = length * 2.f;

		auto negativeOldDirection = VectorMulToScalar(_oldDirection, -1);

		auto returned = SumVectors(VectorMulToScalar(_normalVector, lenVector), _oldDirection);

		switch (_type)
		{
		case PhysicsSystem::CorrectionType::None:
			break;
		case PhysicsSystem::CorrectionType::Upper:
			returned = SumVectors(returned, { 0.f, 0.3f, 0.f });
			break;
		case PhysicsSystem::CorrectionType::Lower:
			returned = SumVectors(returned, { 0.f, -0.3f, 0.f });
			break;
		default:
			break;
		}

		return VectorMulToScalar(NormalizeVector(returned), length);
	}

public:
	PhysicsSystem(Engine* _engine) : System(_engine) {}

	void Update(float dt) override
	{
		for (auto const& entity : mEntities)
		{
			const auto physicsType = engine->GetComponent<PhysicsComponent>(entity);
			const auto itemType = engine->GetComponent<ItemTypeComponent>(entity);
			auto& transform = engine->GetComponent<TransformComponent>(entity);
			auto& direction = engine->GetComponent<DirectionComponent>(entity).direction;
			auto& _speedComponent = engine->GetComponent<SpeedComponent>(entity);

			if (itemType.type == ItemType::Wall)
			{
				for (auto const& testingEntity : mEntities)
				{
					const auto _physicsType = engine->GetComponent<PhysicsComponent>(testingEntity);
					const auto _itemType = engine->GetComponent<ItemTypeComponent>(testingEntity).type;
					auto& _transform = engine->GetComponent<TransformComponent>(testingEntity);
					auto& _direction = engine->GetComponent<DirectionComponent>(testingEntity).direction;

					if (_itemType != ItemType::Player) continue;

					bool hasIntersection = HasIntersection(transform.localPosition.x, 
						transform.localPosition.y,
						physicsType.width, 
						physicsType.height,
						_transform.localPosition.x,
						_transform.localPosition.y,
						_physicsType.width,
						_physicsType.height
					);

					if (hasIntersection && (_direction.y * physicsType.normalVector.y < 0)) _direction = {};
				}
			}

			if (itemType.type == ItemType::Goal)
			{
				for (auto const& testingEntity : mEntities)
				{
					const auto _physicsType = engine->GetComponent<PhysicsComponent>(testingEntity);
					const auto _itemType = engine->GetComponent<ItemTypeComponent>(testingEntity).type;
					auto& _transform = engine->GetComponent<TransformComponent>(testingEntity);
					auto& _direction = engine->GetComponent<DirectionComponent>(testingEntity).direction;

					if (_itemType != ItemType::Ball) continue;

					bool hasIntersection = HasIntersection(
						_transform.localPosition.x,
						_transform.localPosition.y,
						_physicsType.radius,
						transform.localPosition.x,
						transform.localPosition.y,
						physicsType.width,
						physicsType.height);

					if (hasIntersection)
					{
						auto manager = engine->FindEntityByName("GameManager");
						auto& uploadInfo = engine->GetComponent<CountUploadedComponent>(manager);
						uploadInfo = {.side = _transform.localPosition.x > 0? SideUploader::Right : SideUploader::Left, .uploaded = true };
					}
				}
			}

			if (itemType.type == ItemType::Ball)
			{
				for (auto const& testingEntity : mEntities)
				{
					const auto _physicsType = engine->GetComponent<PhysicsComponent>(testingEntity);
					const auto _itemType = engine->GetComponent<ItemTypeComponent>(testingEntity).type;
					const auto _transform = engine->GetComponent<TransformComponent>(testingEntity);
					const auto _direction = engine->GetComponent<DirectionComponent>(testingEntity).direction;

					if (_itemType == ItemType::Ball) continue;

					bool hasIntersection = HasIntersection(
							transform.localPosition.x,
							transform.localPosition.y,
							physicsType.radius,
							_transform.localPosition.x,
							_transform.localPosition.y,
							_physicsType.width,
							_physicsType.height);

					if (hasIntersection)
					{
						_speedComponent.Up(_speedComponent.Get() * .1f);

						CorrectionType type = CorrectionType::None;

						if (_itemType == ItemType::Player)
						{
							if (transform.localPosition.y > (_transform.localPosition.y + (1 / 6) * _physicsType.height))
								type = CorrectionType::Upper;
							else if (transform.localPosition.y < (_transform.localPosition.y - (1 / 6) * _physicsType.height))
								type = CorrectionType::Lower;
						}

						if (IsVectorsEqual(_direction, { 0.f, 0.f, 0.f }))
							direction = SetupNewChangeDirection(direction, _physicsType.normalVector, type);
						else
							direction = SetupNewChangeDirection(direction,
								SumVectors(_physicsType.normalVector, _direction),
								type);
					}
				}
			}
		}
	}
};