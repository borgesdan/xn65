#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "../enums.hpp"
#include "../forward.hpp"
#include "../game/window.hpp"
#include "../types.hpp"
#include "adapter.hpp"
#include "rendertarget.hpp"
#include "swapchain.hpp"
#include "viewport.hpp"
#include "blendstate.hpp"

namespace xna {
	class IGraphicsDevice {
	public:
		virtual ~IGraphicsDevice() {}
		virtual void Clear() = 0;
		virtual void Clear(Color const& color) = 0;
		virtual bool Initialize(GameWindow& gameWindow) = 0;
		virtual bool Present() = 0;
		virtual PGraphicsAdapter Adapter() const = 0;
		virtual void Adapter(PGraphicsAdapter const& adapter) = 0;
		virtual xna::Viewport Viewport() const = 0;
		virtual void Viewport(xna::Viewport const& viewport) = 0;
		virtual void UseVSync(bool use) = 0;		
	};
}

#endif