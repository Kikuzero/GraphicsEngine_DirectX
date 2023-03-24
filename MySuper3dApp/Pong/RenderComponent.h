#pragma once
#include <SimpleMath.h>
#include "../ECS/IComponent.h"
#include "../MySuper3dApp/GraphicsEngine.h"
#include "../MySuper3dApp/ModelManager.h"
#include "../MySuper3dApp/MShader.h"
#include "../MySuper3dApp/VertexBuffer.h"
#include "../MySuper3dApp/IndexBuffer.h"
#include "../MySuper3dApp/ConstBuffer.h"
#include "Material.h"
#include "../Fong/DirectionLightData.h"

#pragma pack(push, 4)
struct ConstMatrixBuffer {
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix world_view_proj;
	DirectX::SimpleMath::Matrix inverse_transpose_world;
};
#pragma pack(pop)

struct RenderComponent : public IComponent
{
	GraphicsEngine* graphicsEngine;

	VertexBuffer* vb;
	IndexBuffer* ib;
	ConstBuffer* cb;
	ConstBuffer* mcb;
	ConstBuffer* lcb;

	Topology topology = Topology::Triangles;

	// TODO: костыл
	RenderComponent() {}

	RenderComponent(GraphicsEngine* _ge)
	{
		auto device = _ge->GetDevice();

		vb = nullptr;
		ib = nullptr;

		// ? TODO
		cb = new ConstBuffer();
		cb->Init<DirectX::SimpleMath::Matrix>();
		cb->Bind(device);

		mcb = new ConstBuffer();
		mcb->Init<Material>();
		mcb->Bind(device);

		lcb = new ConstBuffer();
		lcb->Init<DirectionLightData>();
		lcb->Bind(device);

		topology = Topology::Triangles;
		graphicsEngine = _ge;
	}

	/*
	RenderComponent(GraphicsEngine* _ge, std::vector<DirectX::XMFLOAT4> _verteces, std::vector<int> _indices, Topology _topology, bool _hasLight = false)
	{
		auto device = _ge->GetDevice();

		vb = new VertexBuffer();
		vb->Init<DirectX::XMFLOAT4>(_verteces.data(), _verteces.size());
		vb->AppendStrides(sizeof(DirectX::XMFLOAT4)); // 32
		vb->AppendOffsets(0);
		vb->Bind(device);

		ib = new IndexBuffer();
		ib->Init(_indices.data(), _indices.size());
		ib->Bind(device);

		// ? TODO
		cb = new ConstBuffer();
		cb->Init<DirectX::SimpleMath::Matrix>();
		cb->Bind(device);

		if (_hasLight)
		{
			mcb = new ConstBuffer();
			mcb->Init<Material>();
			mcb->Bind(device);

			lcb = new ConstBuffer();
			lcb->Init<DirectionLightData>();
			lcb->Bind(device);
		}

		topology = _topology;
		graphicsEngine = _ge;
	}
	*/

	
	RenderComponent(GraphicsEngine* _ge, std::vector<ModelManager::Vertex> _verteces, std::vector<int> _indices, Topology _topology)
	{
		auto device = _ge->GetDevice();

		std::cout << sizeof(ModelManager::Vertex) << "\n";

		vb = new VertexBuffer();
		vb->Init<ModelManager::Vertex>(_verteces.data(), _verteces.size());
		vb->AppendStrides(sizeof(ModelManager::Vertex));
		vb->AppendOffsets(0);
		vb->Bind(device);

		ib = new IndexBuffer();
		ib->Init(_indices.data(), _indices.size());
		ib->Bind(device);

		// ? TODO
		cb = new ConstBuffer();
		cb->Init<ConstMatrixBuffer>();
		cb->Bind(device);

		mcb = new ConstBuffer();
		mcb->Init<Material>();
		mcb->Bind(device);

		lcb = new ConstBuffer();
		lcb->Init<DirectionLightData>();
		lcb->Bind(device);

		topology = _topology;
		graphicsEngine = _ge;
	}
	
	/*
	RenderComponent(GraphicsEngine* _ge, VertexBuffer* _vb, IndexBuffer* _ib, ConstBuffer* _cb)
	{
		graphicsEngine = _ge;
		vb = _vb;
		ib = _ib;
		cb = _cb;
	}

	RenderComponent(GraphicsEngine* _ge, std::vector<DirectX::XMFLOAT4> _verteces) 
		: RenderComponent(_ge, _verteces, std::vector<int>(), Topology::Triangles) {}

	RenderComponent(GraphicsEngine* _ge, std::vector<DirectX::XMFLOAT4> _verteces, std::vector<int> _indices) 
		: RenderComponent(_ge, _verteces, _indices, Topology::Triangles) {}

	RenderComponent(GraphicsEngine* _ge, std::vector<DirectX::XMFLOAT4> _verteces, Topology _topology) 
		: RenderComponent(_ge, _verteces, std::vector<int>(), _topology) {}

	RenderComponent(GraphicsEngine* _ge, std::vector<ModelManager::Vertex> _verteces)
		: RenderComponent(_ge, _verteces, std::vector<int>(), Topology::Triangles) {}

	RenderComponent(GraphicsEngine* _ge, std::vector<ModelManager::Vertex> _verteces, std::vector<int> _indices)
		: RenderComponent(_ge, _verteces, _indices, Topology::Triangles) {}

	RenderComponent(GraphicsEngine* _ge, std::vector<ModelManager::Vertex> _verteces, Topology _topology)
		: RenderComponent(_ge, _verteces, std::vector<int>(), _topology) {}
	*/
};