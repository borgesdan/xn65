#ifndef XNA_GRAPHICS_PRESENTPARAMS_HPP
#define XNA_GRAPHICS_PRESENTPARAMS_HPP

#include "shared.hpp"
#include <cstdint>

namespace xna {
	//Defines flags that describe the relationship between the adapter refresh rate and the rate at which Present operations are completed. 
	enum class PresentInterval {
		//Equivalent to setting One.
		Default,
		//The driver waits for the vertical retrace period (the runtime will beam trace to prevent tearing).
		//Present operations are not affected more frequently than the screen refresh rate; 
		//the runtime completes one Present operation per adapter refresh period, at most.
		//This option is always available for both windowed and full-screen swap chains.
		One,
		//The driver waits for the vertical retrace period.
		//Present operations are not affected more frequently than every second screen refresh. 
		Two,
		//The runtime updates the window client area immediately, and might do so more than once during the adapter refresh period.
		//Present operations might be affected immediately.
		//This option is always available for both windowed and full-screen swap chains.
		Immediate
	};

	enum class SwapEffect {
		Discard,
		Sequential,
		FlipSequential,
		FlipDiscard
	};	

	//Contains presentation parameters. 
	struct PresentationParameters {
		//Gets or sets a value indicating the width of the new swap chain's back buffer.
		int32_t BackBufferWidth{ 0 };
		//Gets or sets a value indicating the height of the new swap chain's back buffer.
		int32_t BackBufferHeight{ 0 };
		//Gets or sets the format of the back buffer.
		SurfaceFormat BackBufferFormat{ SurfaceFormat::Color };		
		//Gets or sets the handle to the device window. 
		intptr_t DeviceWindowHandle{ 0 };
		//Gets or sets a value indicating whether the application is in full screen mode.
		bool IsFullscreen{ false };
		//Gets or sets a value indicating the number of sample locations during multisampling.
		int32_t MultiSampleCount{ 0 };
		//Gets or sets the maximum rate at which the swap chain's back buffers can be presented to the front buffer.
		PresentInterval PresentationInterval{ PresentInterval::Default };
		//Gets or sets the depth stencil data format.
		DepthFormat DepthStencilFormat{ DepthFormat::None };

		SwapEffect PresentationSwapEffect{ SwapEffect::FlipDiscard };
	};
}

#endif