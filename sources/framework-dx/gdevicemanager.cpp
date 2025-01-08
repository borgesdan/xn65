#include "xna-dx/framework.hpp"

namespace xna {
	static bool IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter);

	GraphicsDeviceManager::GraphicsDeviceManager(sptr<Game> const& game) : game(game)
	{
		sptr<GraphicsAdapter> adp = GraphicsAdapter::DefaultAdapter();
		information.Adapter = adp;
		information.Profile = xna::GraphicsProfile::HiDef;

		auto parameters = snew<PresentationParameters>();
		parameters->BackBufferWidth = backBufferWidth;
		parameters->BackBufferHeight = backBufferHeight;
		parameters->BackBufferFormat = SurfaceFormat::Color;
		parameters->IsFullscreen = false;
		information.PresentParameters = parameters;	
	}		

	void GraphicsDeviceManager::ApplyChanges() {
		if (device && !isDeviceDirty)
			return;

		ChangeDevice(false);
	}

	bool GraphicsDeviceManager::ToggleFullScreen() {
		if (!game || !game->graphicsDevice || !game->graphicsDevice->Implementation->SwapChain)
			return false;

		auto& swap = game->graphicsDevice->Implementation->SwapChain;

		BOOL state = false;
		auto hr = swap->impl->dxSwapChain->GetFullscreenState(&state, nullptr);

		if (FAILED(hr)) return false;

		hr = swap->impl->dxSwapChain->SetFullscreenState(!state, nullptr);

		if (FAILED(hr)) return false;

		isFullScreen = !state;

		return true;
	}	

	void GraphicsDeviceManager::ChangeDevice(bool forceCreate) {
		if (!game)
			throw csharp::InvalidOperationException();

		inDeviceTransition = true;
		auto screenDeviceName = game->Window()->ScreenDeviceName();
		int clientWidth = game->Window()->ClientBounds().Width;
		int clientHeight = game->Window()->ClientBounds().Height;
		bool flag1 = false;

		//this.game.Window.SetSupportedOrientations(Helpers.ChooseOrientation(this.supportedOrientations, this.PreferredBackBufferWidth, this.PreferredBackBufferHeight, true));
		auto bestDevice = FindBestDevice(forceCreate);
		//this.game.Window.BeginScreenDeviceChange(bestDevice.PresentationParameters.IsFullScreen);
		flag1 = true;
		bool flag2 = true;

		if (!forceCreate && device) {
			//this.OnPreparingDeviceSettings((object) this, new PreparingDeviceSettingsEventArgs(bestDevice));

			if (CanResetDevice(*bestDevice)) {
				auto deviceInformation = snew<GraphicsDeviceInformation>(*bestDevice);
				MassagePresentParameters(*bestDevice->PresentParameters);
				ValidateGraphicsDeviceInformation(*bestDevice);
				device->Reset(deviceInformation->PresentParameters, deviceInformation->Adapter);
				//GraphicsDeviceManager.ConfigureTouchInput(deviceInformation.PresentationParameters);
				flag2 = false;
			}			
		}

		if (flag2)
			CreateDevice(*bestDevice);		

		screenDeviceName = device->Adapter()->DeviceName();
		const auto presentationParameters = device->PresentParameters();

		isReallyFullScreen = presentationParameters->IsFullscreen;

		if (presentationParameters->BackBufferWidth != 0)
			clientWidth = presentationParameters->BackBufferWidth;

		if (presentationParameters->BackBufferHeight != 0)
			clientHeight = presentationParameters->BackBufferHeight;

		isDeviceDirty = false;

		//if (flag1)	game->Window()->EndScreenDeviceChange(screenDeviceName, clientWidth, clientHeight);

		currentWindowOrientation = game->Window()->CurrentOrientation();
		
		inDeviceTransition = false;
	}

	void GraphicsDeviceManager::CreateDevice(GraphicsDeviceInformation& newInfo) {
		if (device)	{			
			device = nullptr;
		}
		
		//this.OnPreparingDeviceSettings((object)this, new PreparingDeviceSettingsEventArgs(newInfo));
		MassagePresentParameters(*newInfo.PresentParameters);
		ValidateGraphicsDeviceInformation(newInfo);

		game->ResizeWindow(newInfo.PresentParameters->BackBufferWidth, newInfo.PresentParameters->BackBufferHeight);

		device = snew<GraphicsDevice>(newInfo.Adapter, newInfo.Profile, newInfo.PresentParameters);
		device->Initialize();

		game->AttachGraphicsDevice(device);

		//device.DeviceResetting += new EventHandler<EventArgs>(this.HandleDeviceResetting);
		//device.DeviceReset += new EventHandler<EventArgs>(this.HandleDeviceReset);
		//device.DeviceLost += new EventHandler<EventArgs>(this.HandleDeviceLost);
		//device.Disposing += new EventHandler<EventArgs>(this.HandleDisposing);

		//GraphicsDeviceManager.ConfigureTouchInput(newInfo.PresentationParameters);
		//this.OnDeviceCreated((object)this, EventArgs.Empty);*/
	}

	void GraphicsDeviceManager::AddDevices(bool anySuitableDevice, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) {
		const auto handle = game->Window()->Handle();
		
		std::vector<uptr<GraphicsAdapter>> adapters;
		GraphicsAdapter::Adapters(adapters);

		for (size_t i = 0; i < adapters.size(); ++i) {
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
					const auto& supportedDisplayModes = adapter->SupportedDisplayModes();
					const auto count = supportedDisplayModes->Count();
					
					for (size_t i = 0; i < count; ++i) {
						auto& supportedDisplayMode = supportedDisplayModes->DisplayModes[i];

						if (supportedDisplayMode->Width() >= 640 && supportedDisplayMode->Height() >= 480) {
							AddDevices(*baseDeviceInfo->Adapter, *supportedDisplayMode, baseDeviceInfo, foundDevices);
						}
					}
				}
			}
		}
	}

	void GraphicsDeviceManager::AddDevices(GraphicsAdapter const& adapter, DisplayMode const& mode, sptr<GraphicsDeviceInformation>& baseDeviceInfo, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) const {
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

	sptr<GraphicsDeviceInformation> GraphicsDeviceManager::FindBestPlatformDevice(bool anySuitableDevice) {
		auto foundDevices = std::vector<sptr<GraphicsDeviceInformation>>();

		AddDevices(anySuitableDevice, foundDevices);

		if (foundDevices.size() == 0 && allowMultiSampling) {
			PreferMultiSampling(false);
			AddDevices(anySuitableDevice, foundDevices);
		}

		if (foundDevices.size() == 0) {
			throw csharp::InvalidOperationException("No Suitable Graphics Device");
		}

		RankDevices(foundDevices);

		if (foundDevices.size() == 0)
			throw csharp::InvalidOperationException("No Suitable Graphics Device");
			
		return foundDevices[0];
	}

	struct GraphicsDeviceInformationComparer
	{
		GraphicsDeviceManager* graphics = nullptr;

		bool operator()(GraphicsDeviceInformation const& d1, GraphicsDeviceInformation const& d2) const {
			return comparator(d1, d2);
		}

		bool operator()(sptr<GraphicsDeviceInformation> const& a, sptr<GraphicsDeviceInformation> const& b) const {
			return comparator(*a, *b);
		}

	private:
		bool comparator(GraphicsDeviceInformation const& d1, GraphicsDeviceInformation const& d2) const {
			if (d1.Profile != d2.Profile)
				return d1.Profile <= d2.Profile;

			auto& presentationParameters1 = d1.PresentParameters;
			auto& presentationParameters2 = d2.PresentParameters;

			if (presentationParameters1 && presentationParameters2 && presentationParameters1->IsFullscreen != presentationParameters2->IsFullscreen)
				return graphics->IsFullScreen() != presentationParameters1->IsFullscreen;

			const auto& backFormat1 = presentationParameters1->BackBufferFormat;
			const auto& backFormat2 = presentationParameters2->BackBufferFormat;

			if (backFormat1 != backFormat2)
				return static_cast<int>(backFormat1) <= static_cast<int>(backFormat2);

			if (presentationParameters1->MultiSampleCount != presentationParameters2->MultiSampleCount)
				return presentationParameters1->MultiSampleCount <= presentationParameters2->MultiSampleCount;

			const auto num3 = graphics->PreferredBackBufferWidth() == 0 || graphics->PreferredBackBufferHeight() == 0 
				? GraphicsDeviceManager::DefaultBackBufferWidth / static_cast<float>(GraphicsDeviceManager::DefaultBackBufferHeight) 
				: graphics->PreferredBackBufferWidth() / static_cast<float>(graphics->PreferredBackBufferHeight());
			
			const auto num4 = presentationParameters1->BackBufferWidth / static_cast<float>(presentationParameters1->BackBufferHeight);
			const auto num5 = presentationParameters2->BackBufferWidth / static_cast<float>(presentationParameters2->BackBufferHeight);
			
			const auto num6 = std::abs(num4 - num3);
			const auto num7 = std::abs(num5 - num3);

			if (std::abs(num6 - num7) > 0.20000000298023224)
				return num6 <= num7;

			Int num8;
			Int num9;
			
			if (graphics->IsFullScreen())
			{
				if (graphics->PreferredBackBufferWidth() == 0 || graphics->PreferredBackBufferHeight() == 0) {
					const auto& adapter1 = d1.Adapter;
					num8 = adapter1->CurrentDisplayMode()->Width() * adapter1->CurrentDisplayMode()->Height();
					const auto& adapter2 = d2.Adapter;
					num9 = adapter2->CurrentDisplayMode()->Width() * adapter2->CurrentDisplayMode()->Height();
				}
				else
					num8 = num9 = graphics->PreferredBackBufferWidth() * graphics->PreferredBackBufferHeight();
			}
			else
				num8 = graphics->PreferredBackBufferWidth() == 0 || graphics->PreferredBackBufferHeight() == 0 
				? (num9 = GraphicsDeviceManager::DefaultBackBufferWidth * GraphicsDeviceManager::DefaultBackBufferHeight) 
				: (num9 = graphics->PreferredBackBufferWidth() * graphics->PreferredBackBufferHeight());

			const auto num10 = std::abs(presentationParameters1->BackBufferWidth * presentationParameters1->BackBufferHeight - num8);
			const auto num11 = std::abs(presentationParameters2->BackBufferWidth * presentationParameters2->BackBufferHeight - num9);

			if (num10 != num11)
				return num10 <= num11;

			if (d1.Adapter != d2.Adapter) {
				if (d1.Adapter->IsDefaultAdapter())
					return false;

				if (d2.Adapter->IsDefaultAdapter())
					return true;
			}

			return false;
		}
	};

	void GraphicsDeviceManager::RankDevicesPlatform(std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) {
		GraphicsDeviceInformationComparer comparer;
		comparer.graphics = this;
		
		std::sort(foundDevices.begin(), foundDevices.end(), comparer);
	}

	bool GraphicsDeviceManager::CanResetDevice(GraphicsDeviceInformation& newDeviceInfo) {
		return device->Profile() == newDeviceInfo.Profile;
	}

	void GraphicsDeviceManager::MassagePresentParameters(PresentationParameters& pp) {
		const auto flag1 = pp.BackBufferWidth == 0;
		const auto flag2 = pp.BackBufferHeight == 0;
		
		if (pp.IsFullscreen)
			return;

		auto hWnd = pp.DeviceWindowHandle;

		if (hWnd == 0) {
			if (!game)
				throw csharp::InvalidOperationException();

			hWnd = game->Window()->Handle();
		}

		/*NativeMethods.RECT rect;
		NativeMethods.GetClientRect(hWnd, out rect);
		if (flag1 && rect.Right == 0)
			pp.BackBufferWidth = 1;
		if (!flag2 || rect.Bottom != 0)
			return;
		pp.BackBufferHeight = 1;*/
	}

	void GraphicsDeviceManager::ValidateGraphicsDeviceInformation(GraphicsDeviceInformation& devInfo) {
		const auto& adapter = devInfo.Adapter;
		auto& presentationParameters = devInfo.PresentParameters;

		if (!presentationParameters->IsFullscreen)
			return;

		if (presentationParameters->BackBufferWidth == 0 || presentationParameters->BackBufferHeight == 0)
			throw csharp::InvalidOperationException();

		bool flag = true;

		const auto& currentDisplayMode = adapter->CurrentDisplayMode();

		if (currentDisplayMode->Format() != presentationParameters->BackBufferFormat && currentDisplayMode->Width() != presentationParameters->BackBufferWidth 
			&& currentDisplayMode->Height() != presentationParameters->BackBufferHeight)
		{
			flag = false;

			const auto& supportedDisplayModes = adapter->SupportedDisplayModes();
			const size_t count = supportedDisplayModes->Count();

			for (size_t i = 0; i < count; ++i) {
				const auto& displayMode = supportedDisplayModes->DisplayModes[i];

				if (displayMode->Width() == presentationParameters->BackBufferWidth && displayMode->Height() == presentationParameters->BackBufferHeight) {
					flag = true;
					break;
				}
			}
		}
		if (!flag)
			throw csharp::InvalidOperationException();
	}

	bool IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter) {
		const auto fromAdapter = GameWindow::ScreenFromAdapter(adapter);
		const auto fromHandle = GameWindow::ScreenFromHandle(windowHandle);

		return (fromAdapter && fromHandle) && (*fromAdapter == *fromHandle);
	}
}