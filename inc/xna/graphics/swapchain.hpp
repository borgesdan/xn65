#ifndef XNA_GRAPHICS_SWAPCHAIN_HPP
#define XNA_GRAPHICS_SWAPCHAIN_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class SwapChain : GraphicsResource {
	public:
		SwapChain();
		SwapChain(sptr<GraphicsDevice> const& device);
		~SwapChain() override;
		bool Initialize();
		bool Present(bool vsync) const;
		bool GetBackBuffer(Texture2D& texture2D);
	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif