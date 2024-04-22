#ifndef XNA_GRAPHICS_SWAPCHAIN_HPP
#define XNA_GRAPHICS_SWAPCHAIN_HPP

#include "../default.hpp"
#include "../game/window.hpp"

namespace xna {
	class ISwapChain {
	public:
		virtual ~ISwapChain() {}
		virtual bool Initialize(GraphicsDevice& device, GameWindow const& gameWindow) = 0;
	};	
}

#endif