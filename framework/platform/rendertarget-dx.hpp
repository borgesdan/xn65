#ifndef XNA_PLATFORM_RENDERTARGET_DX_HPP
#define XNA_PLATFORM_RENDERTARGET_DX_HPP

#include "../graphics/rendertarget.hpp"
#include "texture-dx.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class RenderTarget2D::InternalProperty {
		friend class RenderTarget2D;
		friend class GraphicsDevice;
	public:

	private:
		ID3D11RenderTargetView* _renderTargetView;
	};
}

#endif