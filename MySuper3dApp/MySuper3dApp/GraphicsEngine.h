#pragma once
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Window.h"
#include "Layout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstBuffer.h"
#include "MShader.h"
#include "TransitionConstBuffer.h"

enum class Topology
{
	Triangles,
	Lines
};

class GraphicsEngine
{
public:
	struct WindowRect
	{
		int width;
		int height;
	};

public:
	GraphicsEngine(int _width, int _height)
	{
		sizes = { .width = _width, .height = _height };

		window = new Window(_width, _height);
		hWnd = window->Init();

		if (!hWnd)
		{
			std::cout << "Error while creating window\n";
			return;
		}

		D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

		DXGI_SWAP_CHAIN_DESC swapDesc = {};
		swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Width = _width;
		swapDesc.BufferDesc.Height = _height;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = hWnd;
		swapDesc.Windowed = true;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		auto res = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevel,
			1,
			D3D11_SDK_VERSION,
			&swapDesc,
			&swapChain,
			&device,
			nullptr,
			&context);

		if (FAILED(res))
		{
			std::cout << "Failed while creating device\n";
			return;
		}

		ID3D11Texture2D* backTex;
		res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);
		res = device->CreateRenderTargetView(backTex, nullptr, &rtv);

		ml = new MLayout();
	}

	void SetUpDepthBuffer()
	{
		D3D11_TEXTURE2D_DESC depthTexDesc = {};
		depthTexDesc.ArraySize = 1;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.Width = sizes.width;
		depthTexDesc.Height = sizes.height;
		depthTexDesc.SampleDesc = { 1, 0 };

		device->CreateTexture2D(&depthTexDesc, nullptr, &depthBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStenDesc = {};
		depthStenDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStenDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStenDesc.Flags = 0;

		device->CreateDepthStencilView(depthBuffer, &depthStenDesc, &depthView);
	}

	void GetMouseReady()
	{
		window->GetMouseReady();
	}

	KeyboardState GetKeyboardState()
	{
		return window->GetKeyboardState();
	}

	MouseState* GetMouseState()
	{
		return window->GetMouseState();
	}

	WindowRect GetWindowSizes()
	{
		return sizes;
	}

	Window& GetWindow()
	{
		return *window;
	}

	MShader* GetVertexMShader()
	{
		return vertexMShader;
	}

	MShader* GetPixelMShader()
	{
		return pixelMShader;
	}

	ID3D11VertexShader* GetVertexShader()
	{
		return vertexShader;
	}

	ID3D11PixelShader* GetPixelShader()
	{
		return pixelShader;
	}

	void SetVertexShader(MShader* _vertexShader, std::vector<LayoutCreationDesc> _variableDescs)
	{
		vertexMShader = _vertexShader;
		vertexShader = _vertexShader->GetVertexShader(device);

		ml->Init(device,
			ml->CreateLayoutInput(_variableDescs),
			_variableDescs.size(),
			vertexMShader->GetShaderBufferPointer(),
			vertexMShader->GetShaderBufferSize());
	}

	void SetPixelShader(MShader* _pixelShader)
	{
		pixelMShader = _pixelShader;
		pixelShader = _pixelShader->GetPixelShader(device);
	}

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()
	{
		return device;
	}

	ID3D11DeviceContext* GetContext()
	{
		return context;
	}

	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return rtv;
	}

	IDXGISwapChain* GetSwapChain()
	{
		return swapChain;
	}

	ID3D11InputLayout* GetLayout()
	{
		return ml->GetLayout();
	}

	void CreateVertexShader(const wchar_t* _filePath, D3D_SHADER_MACRO* _macros = nullptr)
	{
		vertexMShader = new MShader(_filePath, "VSMain", "vs_5_0", _macros);
		vertexShader = vertexMShader->GetVertexShader(device);
	}

	void BindVertexShader(std::vector<LayoutCreationDesc> _variableDescs)
	{
		ml->Init(device,
			ml->CreateLayoutInput(_variableDescs),
			_variableDescs.size(),
			vertexMShader->GetShaderBufferPointer(),
			vertexMShader->GetShaderBufferSize());
	}

	void CreatePixelShader(const wchar_t* _filePath, D3D_SHADER_MACRO* _macros = nullptr)
	{
		pixelMShader = new MShader(_filePath, "PSMain", "ps_5_0", _macros);
		pixelShader = pixelMShader->GetPixelShader(device);
	}

	HWND GetWindowDescriptor()
	{
		return hWnd;
	}

	void PreRender()
	{
		RECT rect;
		if (!GetWindowRect(hWnd, &rect)) return;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		CD3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;

		ID3D11RasterizerState* rastState;
		device->CreateRasterizerState(&rastDesc, &rastState);

		context->RSSetState(rastState);

		strides = new UINT[]{ 32 };
		offsets = new UINT[]{ 0 };

		viewport = {};
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1, &viewport);

		float color[] = { 0.f, 0.f, 0.f, 1.0f };
		context->ClearRenderTargetView(rtv, color);
		if(depthView != nullptr)
			context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	template<typename T>
	void UpdateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> _cb, T _constData)
	{
		//D3D11_MAPPED_SUBRESOURCE res = {};
		//context->Map(_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

		//auto dataP = reinterpret_cast<float*>(res.pData);
		//memcpy(dataP, &_constData, sizeof(T));

		//context->Unmap(_cb.Get(), 0);

		context->UpdateSubresource(_cb.Get(), 0, nullptr, &_constData, 0, 0);
	}

	// Depricated
	void Render(Microsoft::WRL::ComPtr<ID3D11Buffer> _vb, Microsoft::WRL::ComPtr<ID3D11Buffer> _cb, Microsoft::WRL::ComPtr<ID3D11Buffer> _ib, size_t ibsize)
	{
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //
		context->IASetIndexBuffer(_ib.Get(), DXGI_FORMAT_R32_UINT, 0); //
		context->IASetVertexBuffers(0, 1, _vb.GetAddressOf(), strides, offsets); //
		context->VSSetConstantBuffers(0, 1, _cb.GetAddressOf());
		
		context->IASetInputLayout(this->GetLayout());
		context->VSSetShader(this->GetVertexShader(), nullptr, 0); // ?
		context->PSSetShader(this->GetPixelShader(), nullptr, 0);  // ?

		context->OMSetRenderTargets(1, &rtv, depthView);

		context->DrawIndexed(ibsize, 0, 0);

		context->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void SetupDrawTopology(Topology _topology)
	{
		switch (_topology)
		{
		case Topology::Lines:
			context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case Topology::Triangles:
			context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		default:
			break;
		}
	}

	void SetupIndexBuffer(IndexBuffer* _ib)
	{
		if (_ib == nullptr) return;

		context->IASetIndexBuffer(_ib->GetBuffer().Get(), _ib->GetFormat(), _ib->GetOffset());
		lastIndexBufferSize = _ib->GetSize();
	}

	void SetupVertexBuffer(VertexBuffer* _vb)
	{
		// TODO:
		// Добавить обертки внутри VetrexBuffer 
		// * StartSlot (возможно не внутри VB, а в функции)
		// * NumBuffers
		context->IASetVertexBuffers(0, 1, _vb->GetBuffer().GetAddressOf(), _vb->GetStrides().data(), _vb->GetOffsets().data());
	}

	void SetupConstBuffer(ConstBuffer* _cb, int _slot = 0)
	{
		if (_cb == nullptr) return;

		context->VSSetConstantBuffers(_slot, 1, _cb->GetBuffer().GetAddressOf());
		context->PSSetConstantBuffers(_slot, 1, _cb->GetBuffer().GetAddressOf());
	}

	void PreDraw()
	{
		context->IASetInputLayout(this->GetLayout());
		context->VSSetShader(this->GetVertexShader(), nullptr, 0);
		context->PSSetShader(this->GetPixelShader(), nullptr, 0);

		context->OMSetRenderTargets(1, &rtv, depthView);
	}

	void Draw(size_t _size = -1)
	{
		PreDraw();

		if (_size == -1) _size = lastIndexBufferSize; // TODO: отдельная переменная? /пока что ладно... кхм/

		assert(_size != -1, "Передайте в функцию количество отрисовываемых точек или задайте индексный буффер: SetupIndexBuffer(IndexBuffer*)");

		context->DrawIndexed(_size, 0, 0);

		PostSraw();
	}

	void PostSraw()
	{
		context->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void AfterRender()
	{
		swapChain->Present(1, 0);
	}

private:
	HWND hWnd;
	IDXGISwapChain* swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* rtv;
	MShader* vertexMShader;
	ID3D11VertexShader* vertexShader;
	MShader* pixelMShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Texture2D* depthBuffer = nullptr;
	ID3D11DepthStencilView* depthView = nullptr;

	D3D11_VIEWPORT viewport;
	Window* window;

	UINT* strides;
	UINT* offsets;

	WindowRect sizes;
	MLayout* ml;

	int lastIndexBufferSize = -1;
};