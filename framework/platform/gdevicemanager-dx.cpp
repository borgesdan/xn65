#include "platform-dx/gdevicemanager-dx.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/game-dx.hpp"
#include "platform-dx/window-dx.hpp"
#include "platform-dx/gdeviceinfo-dx.hpp"
#include "platform-dx/adapter-dx.hpp"
#include "platform-dx/presentparameters-dx.hpp"

namespace xna {
	GraphicsDeviceManager::GraphicsDeviceManager(Game*& game) : _game(game) {		
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();		
		_information.Adapter(adp);
		_information.GraphicsProfile(xna::GraphicsProfile::HiDef);

		PresentationParameters parameters;
		parameters.backBufferWidth = _backBufferWidth;
		parameters.backBufferHeight = _backBufferHeight;
		parameters.backBufferFormat = SurfaceFormat::Color;
		parameters.fullscreen = false;
		_information.PresentationParameters(parameters);

		if(_game) _information.Window(_game->Window());
	}

	bool GraphicsDeviceManager::Initialize() {
		if (!_game)
			return false;

		return CreateDevice();		
	}

	void GraphicsDeviceManager::ApplyChanges() {
	}

	bool GraphicsDeviceManager::ToggleFullScreen() {
		if (!_game || !_game->graphicsDevice || !_game->graphicsDevice->_swapChain)
			return false;

		auto& swap = _game->graphicsDevice->_swapChain;

		BOOL state = false;
		auto hr = swap->dxSwapChain->GetFullscreenState(&state, nullptr);

		if (FAILED(hr)) return false;

		hr = swap->dxSwapChain->SetFullscreenState(!state, nullptr);

		if (FAILED(hr)) return false;

		_isFullScreen = !state;

		return true;
	}

	void GraphicsDeviceManager::PreferredBackBufferWidth(Int value) {
		_backBufferWidth = value;
		_isDeviceDirty = true;
	}

	void GraphicsDeviceManager::PreferredBackBufferHeight(Int value) {
		_backBufferHeight = value;
		_isDeviceDirty = true;
	}

	bool GraphicsDeviceManager::CreateDevice() {
		if (_isDeviceDirty) {
			_information._parameters.backBufferWidth = _backBufferWidth;
			_information._parameters.backBufferHeight = _backBufferHeight;
		}

		auto result = initWindow();

		if (!result) return false; 
		
		return initDevice();
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}

	bool GraphicsDeviceManager::initWindow()
	{
		auto window = _information.Window();

		if (!window) {
			window = _game->Window();
			_information.Window(window);
		}

		window->Size(_backBufferWidth, _backBufferHeight);

		if (!window->Create()) {
			MessageBox(nullptr, "Falha na criação da janela", "XN65", MB_OK);
			return false;
		}

		_information._parameters.windowHandle = window->WindowHandle();

		return true;
	}

	bool GraphicsDeviceManager::initDevice()
	{
		auto window = _information.Window();
		_device = New<GraphicsDevice>(_information);

		if (!_device->Initialize(*window)) {
			MessageBox(window->WindowHandle(), "Falha na inicialização do dispositivo gráfico", "XN65", MB_OK);
			_device = nullptr;
			return false;
		}

		_game->graphicsDevice = _device;

		return true;
	}
}