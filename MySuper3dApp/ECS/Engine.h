#pragma once
#include <windows.h>
#include <memory>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "../MySuper3dApp/GraphicsEngine.h"

enum class SystemTypes {
	RuntimeUpdate,
	RuntimeUnfixed
};

class Engine
{
private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;

	std::shared_ptr<GraphicsEngine> graphicsEngine;

	std::unordered_map<std::string, Entity> mEntities;

	std::vector<std::shared_ptr<System>> mSystems;
	std::vector<std::shared_ptr<System>> mLateSystems;

	Entity player;

	float fixedTimePassed;
public:
	Engine* Init(GraphicsEngine _graphicsEngine)
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();

		graphicsEngine = std::shared_ptr<GraphicsEngine>(&_graphicsEngine);

		return this;
	}

	Entity CreateEntity(std::string _name)
	{
		Entity entity = mEntityManager->CreateEntity();
		mEntities.insert({ _name, entity });

		return entity;
	}

	Entity GetPlayer()
	{
		return player;
	}

	std::string EntityNameCreater(std::string _base)
	{
		std::string name = _base;

		int counter = 0;
		while (1)
		{
			if (FindEntityByName(name) == -1) break;
			name.append(std::to_string(counter));
			counter++;
		}

		return name;
	}

	void SetPlayer(Entity _player)
	{
		player = _player;
	}

	std::shared_ptr<GraphicsEngine> GetGraphicsEngine()
	{
		return graphicsEngine;
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

	Entity FindEntityByName(std::string _name)
	{
		if (mEntities.find(_name) != mEntities.end())
			return mEntities.at(_name);

		return -1;
	}

	std::string FindEntityNameByEntity(Entity _entity)
	{
		for (auto it = mEntities.begin(); it != mEntities.end(); ++it) 
			if (it->second == _entity) return it->first;
		
		return "[N0N3]";
	}

	template<typename T>
	Engine* RegisterSystem(SystemTypes _type = SystemTypes::RuntimeUpdate)
	{
		switch (_type)
		{
		case SystemTypes::RuntimeUpdate:
			mSystems.push_back(mSystemManager->RegisterSystem<T>(this));
			break;
		case SystemTypes::RuntimeUnfixed:
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

	float GetPassedTime()
	{
		return fixedTimePassed;
	}

	void Run()
	{
		using clock = std::chrono::high_resolution_clock;

		using namespace std::chrono_literals;
		constexpr std::chrono::nanoseconds timestep(16ms);

		std::chrono::nanoseconds lag(0ns);
		auto time_start = clock::now();
		auto dt = 0.0016f;

		MSG msg = {};
		while (true) {
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)	break;

			auto delta_time = clock::now() - time_start;

			time_start = clock::now();
			lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

			while (lag >= timestep) {
				lag -= timestep;

				fixedTimePassed += dt;
				for (auto system : mSystems) system->Update(dt);
			}

			for (auto system : mLateSystems) system->Update(dt);
		}
	}
};