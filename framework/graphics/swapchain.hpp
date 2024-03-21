#ifndef XNA_GRAPHICS_SWAPCHAIN_HPP
#define XNA_GRAPHICS_SWAPCHAIN_HPP

#include "../types.hpp"
#include "../enums.hpp"
#include "../forward.hpp"
#include "../game/window.hpp"

namespace xna {
	class ISwapChain {
	public:
		virtual ~ISwapChain() {}
		virtual bool Initialize(GameWindow const& gameWindow) = 0;
		virtual bool Apply() = 0;
	};	
}

#endif