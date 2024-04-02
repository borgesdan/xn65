#ifndef XNA_PLATFORM_RENDERTARGET_DX_HPP
#define XNA_PLATFORM_RENDERTARGET_DX_HPP

#include "../graphics/rendertarget.hpp"
#include "texture-dx.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class RenderTarget2D : public IRenderTarget2D, public Texture2D {
	public:
		RenderTarget2D(GraphicsDevice* device);

		virtual ~RenderTarget2D() override {
			if (_renderTargetView) {
				_renderTargetView->Release();
				_renderTargetView = nullptr;
			}
		}

		virtual bool Apply() override;

	public:
		ID3D11RenderTargetView* _renderTargetView = nullptr;
		GraphicsDevice* _device{ nullptr };
	};
}

#endif