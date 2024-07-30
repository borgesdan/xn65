#ifndef XNA_GRAPHICS_PRESENTPARAMS_HPP
#define XNA_GRAPHICS_PRESENTPARAMS_HPP

#include "../default.hpp"

namespace xna {
	struct PresentationParameters {
		constexpr PresentationParameters() = default;

		Int BackBufferWidth{ 0 };
		Int BackBufferHeight{ 0 };
		SurfaceFormat BackBufferFormat{ SurfaceFormat::Color };
		SwapEffect PresentationSwapEffect{ SwapEffect::FlipDiscard };
		intptr_t DeviceWindowHandle{ 0 };
		bool IsFullscreen{ false };
		Int MultiSampleCount{ 0 };
		PresentInterval PresentationInterval{ PresentInterval::Default };
		DepthFormat DepthStencilFormat{ DepthFormat::None };
	};
}

#endif