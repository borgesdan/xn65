#ifndef XNA_GAME_GDEVICEINFO_HPP
#define XNA_GAME_GDEVICEINFO_HPP

#include "../default.hpp"

namespace xna {
	struct GraphicsDeviceInformation {
        GraphicsDeviceInformation() {
            PresentParameters = snew<PresentationParameters>();
        }

		sptr<GraphicsAdapter> Adapter = nullptr;
		xna::GraphicsProfile Profile{ xna::GraphicsProfile::Reach };
		sptr<xna::PresentationParameters> PresentParameters = nullptr;
		sptr<GameWindow> Window = nullptr;		
	};	
}

#endif