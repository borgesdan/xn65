#ifndef XNA_PLATFORM_RENDERTARGET_DX_HPP
#define XNA_PLATFORM_RENDERTARGET_DX_HPP

#include "../graphics/rendertarget.hpp"
#include "texture-dx.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class RenderTarget2D : public IRenderTarget2D, public Texture2D {
	public:
		virtual ~RenderTarget2D() override {
			if (_renderTargetView) {
				_renderTargetView->Release();
				_renderTargetView = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device) override;
		virtual bool Apply(GraphicsDevice& device) override;

	public:
		ID3D11RenderTargetView* _renderTargetView = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC _renderTargetDesc{};
	};
}

#endif