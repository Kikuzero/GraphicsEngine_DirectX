#pragma once
#include <d3d11.h>
#include <DirectXTex.h>
#include <wrl/client.h>
#include "GraphicsEngine.h"

class Texture 
{
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureView;
public:
	// TODO: вынести в отдельный класс и сделать сеттер mImage
	static DirectX::ScratchImage* LoadTexture(const wchar_t* path, DirectX::ScratchImage* _image)
	{
		LoadFromWICFile(
			path,
			DirectX::WIC_FLAGS_DEFAULT_SRGB,
			nullptr,
			*_image);

		return _image;
	}

	void Init(GraphicsEngine* _ge, DirectX::ScratchImage* _image)
	{
		CreateTextureEx(
			_ge->GetDevice().Get(),
			_image->GetImages(),
			_image->GetImageCount(),
			_image->GetMetadata(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, 0, false,
			reinterpret_cast<ID3D11Resource**>(mTexture.GetAddressOf()));

		_ge->GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, &mTextureView);
	}

	void SetTexture(GraphicsEngine* _ge, int _slot = 0) 
	{
		_ge->GetContext()->PSSetShaderResources(_slot, 1, mTextureView.GetAddressOf());
	}
};