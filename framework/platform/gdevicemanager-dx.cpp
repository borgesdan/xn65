#include "gdevicemanager-dx.hpp"
#include "device-dx.hpp"
#include "game-dx.hpp"
#include "window-dx.hpp"
#include "gdeviceinfo-dx.hpp"

namespace xna {
	void GraphicsDeviceManager::ApplyChanges() {
	}

	void GraphicsDeviceManager::ToggleFullScreen() {
	}

	void GraphicsDeviceManager::CreateDevice(GraphicsDeviceInformation const& info) {
		_device = New<GraphicsDevice>();

		auto window = _game->Window();
		window->Size(_backBufferWidth, _backBufferHeight);

		_device->Initialize(*window);
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}
}