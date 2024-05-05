#include "texture-dx.hpp"
#include "WICTextureLoader.h"
#include "device-dx.hpp"
#include "../helpers.hpp"

namespace xna {	
	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, String const& fileName, xna_error_ptr_arg)
	{		
		auto texture2d = New<Texture2D>(&device);
		ID3D11Resource* resource = nullptr;		
		auto wstr = XnaHToWString(fileName);
		
		HRESULT result = DirectX::CreateWICTextureFromFile(
			device._device,
			device._context,
			wstr.c_str(),
			&resource,
			&texture2d->dxShaderResourceView,
			0U);		

		if (FAILED(result))
		{
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}
		
		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->dxTexture2D);

		if (FAILED(result)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}			

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->dxTexture2D->GetDesc(&desc);
		texture2d->dxDescription = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}

	bool Texture2D::Initialize(xna_error_ptr_arg)
	{
		if (dxTexture2D) {
			xna_error_apply(err, XnaErrorCode::WARNING_INITIALIZED_RESOURCE);
			return false;
		}

		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		auto hr = m_device->_device->CreateTexture2D(&dxDescription, nullptr, &dxTexture2D);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		ID3D11Resource* resource = nullptr;
		hr = dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		hr = m_device->_device->CreateShaderResourceView(resource, &dxShaderDecription, &dxShaderResourceView);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}