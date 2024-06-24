#include "xna/game/gdevicemanager.hpp"
#include "xna/graphics/presentparams.hpp"
#include "xna/graphics/swapchain.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	GraphicsDeviceManager::GraphicsDeviceManager(sptr<Game> const& game) : _game(game)
	{
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();
		_information.Adapter = adp;
		_information.Profile = xna::GraphicsProfile::HiDef;

		auto parameters = snew<PresentationParameters>();
		parameters->BackBufferWidth = _backBufferWidth;
		parameters->BackBufferHeight = _backBufferHeight;
		parameters->BackBufferFormat = SurfaceFormat::Color;
		parameters->Fullscreen = false;
		_information.Parameters = parameters;

		if (_game)
			_information.Window = _game->Window();
	}	

	bool GraphicsDeviceManager::Initialize() {
		if (!_game)
			return false;

		return CreateDevice();		
	}

	void GraphicsDeviceManager::ApplyChanges() {
	}

	bool GraphicsDeviceManager::ToggleFullScreen() {
		if (!_game || !_game->graphicsDevice || !_game->graphicsDevice->impl->_swapChain)
			return false;

		auto& swap = _game->graphicsDevice->impl->_swapChain;

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
	
	bool initWindow(GraphicsDeviceInformation& info, Game& game, int backWidth, int backHeight)
	{
		auto window = info.Window;

		if (!window) {
			window = game.Window();
			info.Window = window;
		}

		window->impl->Size(backWidth, backHeight);

		if (!window->impl->Create()) {
			MessageBox(nullptr, "Falha na criação da janela", "XN65", MB_OK);
			return false;
		}

		info.Parameters->DeviceWindowHandle = reinterpret_cast<intptr_t>(window->impl->WindowHandle());

		return true;
	}

	bool initDevice(GraphicsDeviceInformation& info, Game& game, sptr<GraphicsDevice>& device)
	{		
		device = snew<GraphicsDevice>(info);

		if (!device->Initialize()) {
			MessageBox(info.Window->impl->WindowHandle(), "Falha na inicialização do dispositivo gráfico", "XN65", MB_OK);
			device = nullptr;
			return false;
		}

		game.graphicsDevice = device;

		return true;
	}


	bool GraphicsDeviceManager::CreateDevice() {
		if (_isDeviceDirty) {
			_information.Parameters->BackBufferWidth = _backBufferWidth;
			_information.Parameters->BackBufferHeight = _backBufferHeight;
		}

		auto result = initWindow(_information, *_game, _backBufferWidth, _backBufferHeight);

		if (!result) return false; 
		
		return initDevice(_information, *_game, _device);
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}

	
}