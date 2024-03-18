#ifndef XNA_PLATFORM_SWAPCHAIN_DX_HPP
#define XNA_PLATFORM_SWAPCHAIN_DX_HPP

#include "../graphics/swapchain.hpp"
#include "window-dx.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class SwapChain::InternalProperty {
		friend class SwapChain;
		friend class GraphicsDevice;
	public:
		InternalProperty(){}

		~InternalProperty() {
			if (_swapChain) {
				_swapChain->Release();
				_swapChain = nullptr;
			}
		}

		bool GetBackBuffer(ID3D11Texture2D*& texture2D) {
			if FAILED(_swapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D)))
				return false;

			return true;
		}

	private:
		IDXGISwapChain* _swapChain{nullptr};
		DXGI_SWAP_CHAIN_DESC _swapDescription{};
	};
}

#endif