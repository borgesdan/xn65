#ifndef XNA_PLATFORM_SWAPCHAIN_DX_HPP
#define XNA_PLATFORM_SWAPCHAIN_DX_HPP

#include "../graphics/swapchain.hpp"
#include "window-dx.hpp"
#include "dxheaders.hpp"

namespace xna {
	class SwapChain : public ISwapChain{
	public:
		virtual ~SwapChain() override {
			if (dxSwapChain) {
				dxSwapChain->Release();
				dxSwapChain = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, GameWindow const& gameWindow) override;
		bool Initialize(GraphicsDevice& device, GameWindow const& gameWindow, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fullScreenDesc);

		bool GetBackBuffer(ID3D11Texture2D*& texture2D);

		bool Present(bool vsync);

	public:
		IDXGISwapChain1* dxSwapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC1 dxDescription{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxFullScreenDescription{};	
	};	
}

#endif