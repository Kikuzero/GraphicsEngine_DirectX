#pragma once
#include <array>
#include <queue>
#include "Entity.h"
#include "Signature.h"

class EntityManager
{
private:
	std::queue<Entity> mAvailableEntities{};
	std::array<Signature, MAX_ENTITIES> mSignatures{};

	uint32_t mLivingEntityCount{};

public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			mAvailableEntities.push(entity);
	}

	Entity CreateEntity()
	{
		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		mSignatures[entity].reset();

		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		mSignatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		return mSignatures[entity];
	}
};