#include "xna/xna-dx.hpp"

namespace xna {
	GraphicsDeviceManager::GraphicsDeviceManager(sptr<Game> const& game) : game(game)
	{
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();
		_information.Adapter = adp;
		_information.Profile = xna::GraphicsProfile::HiDef;

		auto parameters = snew<PresentationParameters>();
		parameters->BackBufferWidth = backBufferWidth;
		parameters->BackBufferHeight = backBufferHeight;
		parameters->BackBufferFormat = SurfaceFormat::Color;
		parameters->Fullscreen = false;
		_information.Parameters = parameters;

		if (game)
			_information.Window = game->Window();
	}	

	bool GraphicsDeviceManager::Initialize() {
		if (!game)
			return false;

		CreateDevice();

		return true;
	}

	void GraphicsDeviceManager::ApplyChanges() {
		if (device && !isDeviceDirty)
			return;

		ChangeDevice(false);
	}

	bool GraphicsDeviceManager::ToggleFullScreen() {
		if (!game || !game->graphicsDevice || !game->graphicsDevice->impl->_swapChain)
			return false;

		auto& swap = game->graphicsDevice->impl->_swapChain;

		BOOL state = false;
		auto hr = swap->impl->dxSwapChain->GetFullscreenState(&state, nullptr);

		if (FAILED(hr)) return false;

		hr = swap->impl->dxSwapChain->SetFullscreenState(!state, nullptr);

		if (FAILED(hr)) return false;

		isFullScreen = !state;

		return true;
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


	void GraphicsDeviceManager::CreateDevice() {
		if (isDeviceDirty) {
			_information.Parameters->BackBufferWidth = backBufferWidth;
			_information.Parameters->BackBufferHeight = backBufferHeight;
		}

		auto result = initWindow(_information, *game, backBufferWidth, backBufferHeight);

		//if (!result) return false; 
		
		initDevice(_information, *game, device);
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}

	void GraphicsDeviceManager::AddDevice(bool anySuitableDevice, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) {
		const auto handle = game->Window()->Handle();
		
		std::vector<uptr<GraphicsAdapter>> adapters;
		GraphicsAdapter::Adapters(adapters);

		for (size_t i = 0; adapters.size(); ++i) {
			auto& adapter = adapters[i];

			if (!anySuitableDevice) {
				//TODO
				//if (!this.IsWindowOnAdapter(handle, adapter))
				//continue;
			}
		}
	}
}