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
	// Map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> mSignatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem(Engine* _engine)
	{
		const char* typeName = typeid(T).name();

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>(_engine);
		mSystems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		// Set the signature for this system
		mSignatures.insert({ typeName, signature });
	}

	template<typename T>
	Signature GetSignature()
	{
		const char* typeName = typeid(T).name();

		Signature signature;

		if (mSignatures.find(typeName) == mSignatures.end())
		{
			mSignatures.insert({ typeName, signature });
		}

		return mSignatures.at(typeName);
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}
};