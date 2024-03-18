#ifndef XNA_PLATFORM_TEXTURE_DX_HPP
#define XNA_PLATFORM_TEXTURE_DX_HPP

#include "../graphics/texture.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class Texture2D::InternalProperty {
		friend class Texture2D;
		friend class GraphicsDevice;	
		friend class RenderTarget2D;
	public:		

	private:
		ID3D11Texture2D* _texture2D{nullptr};
	};
}

#endif