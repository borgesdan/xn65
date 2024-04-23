#include "gdevicemanager-dx.hpp"
#include "device-dx.hpp"
#include "game-dx.hpp"
#include "window-dx.hpp"
#include "gdeviceinfo-dx.hpp"
#include "adapter-dx.hpp"

namespace xna {
	GraphicsDeviceManager::GraphicsDeviceManager(Game*& game) : _game(game) {		
	}

	bool GraphicsDeviceManager::Initialize() {
		GraphicsDeviceInformation information;

		auto adp = GraphicsAdapter::DefaultAdapter();
		const PGraphicsAdapter sadp = std::move(adp);
		information.Adapter(sadp);
		information.GraphicsProfile(xna::GraphicsProfile::HiDef);

		PresentationParameters parameters;
		parameters.BackBufferWidth = _backBufferWidth;
		parameters.BackBufferHeight = _backBufferHeight;
		parameters.BackBufferFormat = SurfaceFormat::Color;
		parameters.IsFullScreen = false;
		information.PresentationParameters(parameters);

		information.Window(_game->Window());

		CreateDevice(information);

		return true;
	}

	void GraphicsDeviceManager::ApplyChanges() {
	}

	void GraphicsDeviceManager::ToggleFullScreen() {
		if (!_game || !_game->_graphicsDevice || !_game->_graphicsDevice->_swapChain)
			return;

		auto& swap = _game->_graphicsDevice->_swapChain;

		BOOL state = false;
		auto hr = swap->dxSwapChain->GetFullscreenState(&state, nullptr);

		if (FAILED(hr)) return;

		hr = swap->dxSwapChain->SetFullscreenState(!state, nullptr);

		if (FAILED(hr)) return;

		_ifFullScreen = !state;
	}

	void GraphicsDeviceManager::CreateDevice(GraphicsDeviceInformation const& info) {
		_device = New<GraphicsDevice>(info);
		auto window = info.Window();		

		window->Size(_backBufferWidth, _backBufferHeight);
		
		if (!window->Create()) {
			MessageBox(nullptr, "Falha na criação da janela", "Xna Game Engine", MB_OK);			
			return;
		}
		
		if (!_device->Initialize(*window)) {
			MessageBox(nullptr, "Falha na inicialização do dispositivo gráfico", "Xna Game Engine", MB_OK);			
			return;
		}		

		_game->_graphicsDevice = _device;
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}
}