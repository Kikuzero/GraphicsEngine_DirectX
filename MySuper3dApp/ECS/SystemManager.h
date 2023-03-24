#pragma once
#include <memory>
#include <unordered_map>
#include "Signature.h"
#include "Entity.h"
#include "System.h"

class Engine;

class SystemManager
{
private:
	std::unordered_map<const char*, Signature> mSignatures{};
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem(Engine* _engine)
	{
		const char* typeName = typeid(T).name();

		auto system = std::make_shared<T>(_engine);
		mSystems.insert({ typeName, system });

		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		mSignatures.insert_or_assign(typeName, signature);
	}

	template<typename T>
	Signature GetSignature()
	{
		const char* typeName = typeid(T).name();

		Signature signature;

		if (mSignatures.find(typeName) == mSignatures.end())
			mSignatures.insert_or_assign(typeName, signature);

		return mSignatures.at(typeName);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
				system->mEntities.insert(entity);
			else
				system->mEntities.erase(entity);
		}
	}
};