#pragma once
#include <memory>
#include <chrono>
#include <vector>
#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

enum class SystemTypes {
	Begin,
	End,
	RuntimeUpdate,
	RuntimeLate
};

class Engine
{
private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;

	std::vector<std::shared_ptr<System>> mSystems;
	std::vector<std::shared_ptr<System>> mLateSystems;
	std::vector<std::shared_ptr<System>> mBeginSystems;
	std::vector<std::shared_ptr<System>> mEndSystems;
public:
	Engine* Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();

		return this;
	}

	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);
	}

	template<typename Tail>
	Engine* RegisterComponent()
	{
		mComponentManager->RegisterComponent<Tail>();

		return this;
	}

	template<typename Head, typename Second, typename... Tail>
	Engine* RegisterComponent()
	{
		mComponentManager->RegisterComponent<Head>();

		this->RegisterComponent<Second, Tail...>();

		return this;
	}

	template<typename T>
	Engine* AddComponents(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);

		return this;
	}

	template<typename Head, typename Second, typename... Tail>
	Engine* AddComponents(Entity entity, Head headComponent, Second secondComponent, Tail... components)
	{
		mComponentManager->AddComponent<Head>(entity, headComponent);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<Head>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);

		this->AddComponents<Second, Tail...>(entity, secondComponent, components...);

		return this;
	}

	template<typename T>
	Engine* RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);

		return this;
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	template<typename T>
	Engine* RegisterSystem(SystemTypes _type = SystemTypes::RuntimeUpdate)
	{
		switch (_type)
		{
		case SystemTypes::Begin:
			mBeginSystems.push_back(mSystemManager->RegisterSystem<T>(this));
			break;
		case SystemTypes::End:
			mEndSystems.push_back(mSystemManager->RegisterSystem<T>(this));
			break;
		case SystemTypes::RuntimeUpdate:
			mSystems.push_back(mSystemManager->RegisterSystem<T>(this));
			break;
		case SystemTypes::RuntimeLate:
			mLateSystems.push_back(mSystemManager->RegisterSystem<T>(this));
			break;
		default:
			break;
		}

		return this;
	}

	template<typename System, typename Component>
	Engine* AddComponentsToSystemNode()
	{
		Signature signature = mSystemManager->GetSignature<System>();

		signature.set(this->GetComponentType<Component>(), true);

		mSystemManager->SetSignature<System>(signature);

		return this;
	}

	template<typename System, typename HeadComponent, typename SecondComponent, typename... TailComponent>
	Engine* AddComponentsToSystemNode()
	{
		Signature signature = mSystemManager->GetSignature<System>();

		signature.set(this->GetComponentType<HeadComponent>(), true);

		mSystemManager->SetSignature<System>(signature);

		this->AddComponentsToSystemNode<System, SecondComponent, TailComponent...>();

		return this;
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

	void Run()
	{
		using clock = std::chrono::high_resolution_clock;

		using namespace std::chrono_literals;
		constexpr std::chrono::nanoseconds timestep(16ms);

		std::chrono::nanoseconds lag(0ns);
		auto time_start = clock::now();
		auto dt = 0.f;

		for (auto system : mBeginSystems)
		{
			system->Update(dt);
		}

		while (true) {
			auto delta_time = clock::now() - time_start;
			dt = delta_time.count();

			time_start = clock::now();
			lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

			while (lag >= timestep) {
				lag -= timestep;

				for (auto system : mSystems)
				{
					system->Update(0.0016f);
				}
			}

			for (auto system : mLateSystems)
			{
				system->Update(dt);
			}
		}

		for (auto system : mEndSystems)
		{
			system->Update(dt);
		}
	}
};