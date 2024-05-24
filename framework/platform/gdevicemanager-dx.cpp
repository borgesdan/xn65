#include "platform-dx/gdevicemanager-dx.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/game-dx.hpp"
#include "graphics/presentparams.hpp"
#include "graphics/swapchain.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	GraphicsDeviceManager::GraphicsDeviceManager(Game*& game) : _game(game) {		
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();		
		_information.Adapter = adp;
		_information.Profile = xna::GraphicsProfile::HiDef;

		auto parameters = snew<PresentationParameters>();
		parameters->BackBufferWidth = _backBufferWidth;
		parameters->BackBufferHeight = _backBufferHeight;
		parameters->BackBufferFormat = SurfaceFormat::Color;
		parameters->Fullscreen = false;
		_information.Parameters = parameters;

		if(_game) 
			_information.Window =_game->Window();
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
		auto hr = swap->impl->dxSwapChain->GetFullscreenState(&state, nullptr);

		if (FAILED(hr)) return false;

		hr = swap->impl->dxSwapChain->SetFullscreenState(!state, nullptr);

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
			_information.Parameters->BackBufferWidth = _backBufferWidth;
			_information.Parameters->BackBufferHeight = _backBufferHeight;
		}

		auto result = initWindow();

		if (!result) return false; 
		
		return initDevice();
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}

	bool GraphicsDeviceManager::initWindow()
	{
		auto window = _information.Window;

		if (!window) {
			window = _game->Window();
			_information.Window = window;
		}

		window->impl->Size(_backBufferWidth, _backBufferHeight);

		if (!window->impl->Create()) {
			MessageBox(nullptr, "Falha na criação da janela", "XN65", MB_OK);
			return false;
		}

		_information.Parameters->DeviceWindowHandle = reinterpret_cast<intptr_t>(window->impl->WindowHandle());

		return true;
	}

	bool GraphicsDeviceManager::initDevice()
	{
		auto window = _information.Window;
		_device = New<GraphicsDevice>(_information);

		if (!_device->Initialize(*window)) {
			MessageBox(window->impl->WindowHandle(), "Falha na inicialização do dispositivo gráfico", "XN65", MB_OK);
			_device = nullptr;
			return false;
		}

		_game->graphicsDevice = _device;

		return true;
	}
}