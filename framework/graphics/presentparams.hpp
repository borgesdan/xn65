#ifndef XNA_GRAPHICS_PRESENTPARAMS_HPP
#define XNA_GRAPHICS_PRESENTPARAMS_HPP

#include "../default.hpp"

namespace xna {
	class IPresentationParameters {	
		virtual Uint BackBufferWidth() const = 0;
		virtual Uint BackBufferHeight() const = 0;
		virtual SurfaceFormat BackBufferFormat() const = 0;
		virtual SwapEffect PresentationSwapEffect() const = 0;
		virtual intptr_t DeviceWindowHandle() const = 0;
		virtual bool IsFullScreen() const = 0;
	};
}

#endif