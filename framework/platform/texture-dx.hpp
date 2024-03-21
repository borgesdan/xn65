#ifndef XNA_PLATFORM_TEXTURE_DX_HPP
#define XNA_PLATFORM_TEXTURE_DX_HPP

#include "../graphics/texture.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class Texture2D : public ITexture2D {
	public:
		Texture2D();

		virtual ~Texture2D() override {
			if (_texture2D) {
				_texture2D->Release();
				_texture2D = nullptr;
			}
		}

	public:
		ID3D11Texture2D* _texture2D{nullptr};
	};
}

#endif