#ifndef XNA_PLATFORM_SWAPCHAIN_DX_HPP
#define XNA_PLATFORM_SWAPCHAIN_DX_HPP

#include "../graphics/swapchain.hpp"
#include "window-dx.hpp"
#include "dxheaders.hpp"

namespace xna {
	class SwapChain : public ISwapChain{
	public:
		virtual ~SwapChain() override {
			if (_swapChain) {
				_swapChain->Release();
				_swapChain = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, GameWindow const& gameWindow) override;
		bool Initialize(GraphicsDevice& device, GameWindow const& gameWindow, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fullScreenDesc);

		bool GetBackBuffer(ID3D11Texture2D*& texture2D) {
			if FAILED(_swapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D)))
				return false;

			return true;
		}

	public:
		IDXGISwapChain1* _swapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC1 _description{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC _fullScreenDescription{};	
	};	
}

#endif