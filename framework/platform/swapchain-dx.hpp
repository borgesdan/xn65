#ifndef XNA_PLATFORM_SWAPCHAIN_DX_HPP
#define XNA_PLATFORM_SWAPCHAIN_DX_HPP

#include "../graphics/swapchain.hpp"
#include "window-dx.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class SwapChain : public ISwapChain{
	public:
		SwapChain(GraphicsDevice* device);

		virtual ~SwapChain() override {
			if (_swapChain) {
				_swapChain->Release();
				_swapChain = nullptr;
			}
		}

		virtual bool Initialize(GameWindow const& gameWindow) override;
		virtual bool Apply() override;

		bool GetBackBuffer(ID3D11Texture2D*& texture2D) {
			if FAILED(_swapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D)))
				return false;

			return true;
		}

	public:
		IDXGISwapChain* _swapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC _swapDescription{};	
		GraphicsDevice* _device{ nullptr };
	};	
}

#endif