#ifndef XNA_GRAPHICS_PRESENTPARAMS_HPP
#define XNA_GRAPHICS_PRESENTPARAMS_HPP

#include "../default.hpp"

namespace xna {
	//Contains presentation parameters. 
	struct PresentationParameters {
		//Gets or sets a value indicating the width of the new swap chain's back buffer.
		Int BackBufferWidth{ 0 };
		//Gets or sets a value indicating the height of the new swap chain's back buffer.
		Int BackBufferHeight{ 0 };
		//Gets or sets the format of the back buffer.
		SurfaceFormat BackBufferFormat{ SurfaceFormat::Color };
		SwapEffect PresentationSwapEffect{ SwapEffect::FlipDiscard };
		//Gets or sets the handle to the device window. 
		intptr_t DeviceWindowHandle{ 0 };
		//Gets or sets a value indicating whether the application is in full screen mode.
		bool IsFullscreen{ false };
		//Gets or sets a value indicating the number of sample locations during multisampling.
		Int MultiSampleCount{ 0 };
		//Gets or sets the maximum rate at which the swap chain's back buffers can be presented to the front buffer.
		PresentInterval PresentationInterval{ PresentInterval::Default };
		//Gets or sets the depth stencil data format.
		DepthFormat DepthStencilFormat{ DepthFormat::None };
	};
}

#endif