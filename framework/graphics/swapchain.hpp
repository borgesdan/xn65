#ifndef XNA_GRAPHICS_SWAPCHAIN_HPP
#define XNA_GRAPHICS_SWAPCHAIN_HPP

#include "../types.hpp"
#include "../enums.hpp"
#include "../forward.hpp"
#include "../game/window.hpp"

namespace xna {
	class SwapChain {
	public:
		PLATFORM_DEVELOPMENT
			SwapChain(GraphicsDevice* device);

		PLATFORM_DEVELOPMENT
		bool Initialize(GameWindow const& gameWindow);

		PLATFORM_DEVELOPMENT
			bool Apply();	

	private:
		GraphicsDevice* _device{ nullptr };		

	public:
		class InternalProperty;
		friend class InternalProperty;	
		sptr<InternalProperty> ip_SwapChain{ nullptr };
	};
}

#endif