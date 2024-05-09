#include "platform-dx/presentparameters-dx.hpp"
#include "platform-dx/gdevicemanager-dx.hpp"

namespace xna {
	PresentationParameters::PresentationParameters() :
		backBufferWidth(GraphicsDeviceManager::DefaultBackBufferWidth),
		backBufferHeight(GraphicsDeviceManager::DefaultBackBufferHeight) {
	}
}


