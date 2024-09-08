#ifndef XNA_GAME_GDEVICEINFO_HPP
#define XNA_GAME_GDEVICEINFO_HPP

#include "../default.hpp"

namespace xna {
	//Holds the settings for creating a graphics device on Windows. 
	struct GraphicsDeviceInformation {
        GraphicsDeviceInformation() {
            PresentParameters = snew<PresentationParameters>();
        }

		//Specifies which graphics adapter to create the device on.
		sptr<GraphicsAdapter> Adapter = nullptr;
		//Gets the graphics profile, which determines the graphics feature set.
		xna::GraphicsProfile Profile{ xna::GraphicsProfile::Reach };
		//Specifies the presentation parameters to use when creating a graphics device. 
		sptr<xna::PresentationParameters> PresentParameters = nullptr;
	};	
}

#endif