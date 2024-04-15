#include "texture-dx.hpp"
#include "WICTextureLoader.h"
#include "device-dx.hpp"
#include "../helpers.hpp"

namespace xna {
	Texture2D::Texture2D() {		
	}	

	PTexture2D Texture2D::FromStream(GraphicsDevice& device, String const& fileName, xna_error_ptr_arg)
	{		
		auto texture2d = New<Texture2D>();
		ID3D11Resource* resource = nullptr;		
		auto wstr = XnaHToWString(fileName);
		
		HRESULT result = DirectX::CreateWICTextureFromFile(
			device._device,
			device._context,
			wstr.c_str(),
			&resource,
			&texture2d->_textureView,
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
		
		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->_texture2D);

		if (FAILED(result)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}			

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->_texture2D->GetDesc(&desc);
		texture2d->_description = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}
}