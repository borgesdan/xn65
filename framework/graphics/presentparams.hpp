#ifndef XNA_GRAPHICS_PRESENTPARAMS_HPP
#define XNA_GRAPHICS_PRESENTPARAMS_HPP

#include "../types.hpp"
#include "../enums.hpp"

namespace xna {
	class PresentationParameters {
	public:
		Int BackBufferWidth{ 0 };
		Int BackBufferHeight{ 0 };
		SurfaceFormat BackBufferFormat{ SurfaceFormat::Color };
		DepthFormat DepthStencilFormat{ DepthFormat::None };
		Int MultiSampleCount{ 0 };
		xna::DisplayOrientation DisplayOrientation{ xna::DisplayOrientation::Default };
		PresentInterval PresentationInterval{ PresentInterval::Default };
		xna::RenderTargetUsage RenderTargetUsage{ xna::RenderTargetUsage::DiscardContents };
		intptr_t DeviceWindowHandle{ 0 };
		bool IsFullScreen{ false };
	};
}

#endif