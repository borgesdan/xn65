#include "xna/xna-dx.hpp"

namespace xna {
	static bool IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter);

	GraphicsDeviceManager::GraphicsDeviceManager(sptr<Game> const& game) : game(game)
	{
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();
		_information.Adapter = adp;
		_information.Profile = xna::GraphicsProfile::HiDef;

		auto parameters = snew<PresentationParameters>();
		parameters->BackBufferWidth = backBufferWidth;
		parameters->BackBufferHeight = backBufferHeight;
		parameters->BackBufferFormat = SurfaceFormat::Color;
		parameters->IsFullscreen = false;
		_information.PresentParameters = parameters;

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

		info.PresentParameters->DeviceWindowHandle = reinterpret_cast<intptr_t>(window->impl->WindowHandle());

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
			_information.PresentParameters->BackBufferWidth = backBufferWidth;
			_information.PresentParameters->BackBufferHeight = backBufferHeight;
		}

		auto result = initWindow(_information, *game, backBufferWidth, backBufferHeight);

		//if (!result) return false; 
		
		initDevice(_information, *game, device);
	}

	void GraphicsDeviceManager::ChangeDevice() {
	}

	void GraphicsDeviceManager::AddDevices(bool anySuitableDevice, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) {
		const auto handle = game->Window()->Handle();
		
		std::vector<uptr<GraphicsAdapter>> adapters;
		GraphicsAdapter::Adapters(adapters);

		for (size_t i = 0; adapters.size(); ++i) {
			auto& adapter = adapters[i];

			if (!anySuitableDevice) {				
				if (!IsWindowOnAdapter(handle, *adapter))
				continue;
			}

			if (adapter->IsProfileSupported(graphicsProfile)) {
				auto baseDeviceInfo = snew<GraphicsDeviceInformation>();
				baseDeviceInfo->Adapter = std::move(adapter);
				baseDeviceInfo->Profile = graphicsProfile;
				baseDeviceInfo->PresentParameters = snew<PresentationParameters>();
				baseDeviceInfo->PresentParameters->DeviceWindowHandle = handle;
				baseDeviceInfo->PresentParameters->MultiSampleCount = 0;
				baseDeviceInfo->PresentParameters->IsFullscreen = isFullScreen;
				baseDeviceInfo->PresentParameters->PresentationInterval = synchronizeWithVerticalRetrace ? PresentInterval::One : PresentInterval::Immediate;

				const auto& currentDisplayMode = baseDeviceInfo->Adapter->CurrentDisplayMode();
				AddDevices(*baseDeviceInfo->Adapter, *currentDisplayMode, baseDeviceInfo, foundDevices);

				if (isFullScreen) {
					//TODO
				}
			}
		}
	}

	void GraphicsDeviceManager::AddDevices(GraphicsAdapter const& adapter, DisplayMode const& mode, sptr<GraphicsDeviceInformation>& baseDeviceInfo, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) {
		auto deviceInformation = snew<GraphicsDeviceInformation>(*baseDeviceInfo);

		if (isFullScreen)
		{
			deviceInformation->PresentParameters->BackBufferWidth = mode.Width();
			deviceInformation->PresentParameters->BackBufferHeight = mode.Height();
		} 
		else if (useResizedBackBuffer) {
			deviceInformation->PresentParameters->BackBufferWidth = resizedBackBufferWidth;
			deviceInformation->PresentParameters->BackBufferHeight = resizedBackBufferHeight;
		}
		else {
			deviceInformation->PresentParameters->BackBufferWidth = backBufferWidth;
			deviceInformation->PresentParameters->BackBufferHeight = backBufferHeight;
		}

		SurfaceFormat selectedFormat;
		DepthFormat selectedDepthFormat;
		int selectedMultiSampleCount;

		adapter.QueryBackBufferFormat(deviceInformation->Profile, mode.Format(), depthStencilFormat, allowMultiSampling ? 16 : 0, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);

		deviceInformation->PresentParameters->BackBufferFormat = selectedFormat;
		deviceInformation->PresentParameters->DepthStencilFormat = selectedDepthFormat;
		deviceInformation->PresentParameters->MultiSampleCount = selectedMultiSampleCount;

		if (std::find(foundDevices.begin(), foundDevices.end(), deviceInformation) != foundDevices.end())
			return;

		foundDevices.push_back(deviceInformation);
	}

	bool IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter) {
		return GameWindow::ScreenFromAdapter(adapter) == GameWindow::ScreenFromHandle(windowHandle);
	}
}