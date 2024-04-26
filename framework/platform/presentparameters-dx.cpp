#include "presentparameters-dx.hpp"
#include "gdevicemanager-dx.hpp"

namespace xna {
	PresentationParameters::PresentationParameters() :
		backBufferWidth(GraphicsDeviceManager::DefaultBackBufferWidth),
		backBufferHeight(GraphicsDeviceManager::DefaultBackBufferHeight) {
	}
}


