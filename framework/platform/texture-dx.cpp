#include "texture-dx.hpp"
#include "WICTextureLoader.h"
#include "device-dx.hpp"
#include "../helpers.hpp"

namespace xna {
	Texture2D::Texture2D() {		
	}

	PTexture2D Texture2D::FromStream(GraphicsDevice& device, String const& fileName, xna_error_ptr_arg)
	{		
		ID3D11Resource* resource = nullptr;
		//D3D11ShaderResourceView* view = nullptr;
		auto wstr = StringToWString(fileName);
		
		HRESULT result = DirectX::CreateWICTextureFromFile(
			device._device, device._context, wstr.c_str(),
			&resource, nullptr, 0U);

		if (FAILED(result))
		{
			xna_error_apply(err, XnaErrorCode::STREAM_ERROR);
			
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}

		ID3D11Texture2D* txt2d = nullptr;
		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&txt2d);

		if (FAILED(result)) {
			xna_error_apply(err, XnaErrorCode::BAD_CAST);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}			

			return nullptr;
		}

		auto texture2d = New<Texture2D>();
		texture2d->_texture2D = txt2d;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}
}