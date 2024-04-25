#ifndef XNA_GRAPHICS_SWAPCHAIN_HPP
#define XNA_GRAPHICS_SWAPCHAIN_HPP

#include "../default.hpp"
#include "../game/window.hpp"

namespace xna {
	class ISwapChain {
	public:
		virtual ~ISwapChain() {}
		virtual bool Initialize(GameWindow const& gameWindow, xna_error_nullarg) = 0;
	};	
}

#endif