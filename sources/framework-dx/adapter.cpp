#include "xna-dx/framework.hpp"

namespace xna {	
	static void setOutputVars(comptr<IDXGIAdapter1> const& adapter, String& deviceName, intptr_t& monitorHandle);
	static size_t getDisplayModesCount(IDXGIAdapter* adapter);
	static sptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC1> const& source);
	static void setCurrentDisplayMode(DisplayModeCollection& displayModes, SurfaceFormat surfaceFormat, Uint width, Uint height, sptr<DisplayMode>& currentDisplayMode);
	static sptr<DisplayModeCollection> getSupportedDisplayModes(comptr<IDXGIAdapter1>& dxAdapter);

	GraphicsAdapter::GraphicsAdapter() {
		impl = unew<PlatformImplementation>();
	}

	uptr<GraphicsAdapter> GraphicsAdapter::DefaultAdapter() {
		comptr<IDXGIFactory1> pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.GetAddressOf()))
			Exception::Throw(Exception::FAILED_TO_CREATE);

		comptr<IDXGIAdapter1> pAdapter = nullptr;

		if (pFactory->EnumAdapters1(0, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			auto adp = uptr<GraphicsAdapter>(new GraphicsAdapter());

			adp->impl->adapter = pAdapter;
			adp->impl->factory = pFactory;

			DXGI_ADAPTER_DESC1 desc{};
			pAdapter->GetDesc1(&desc);

			adp->description = XnaHelper::ToString(desc.Description);
			adp->deviceId = static_cast<Uint>(desc.DeviceId);
			adp->isDefault = true;
			adp->revision = static_cast<Uint>(desc.Revision);
			adp->subSystemId = static_cast<Uint>(desc.SubSysId);
			adp->vendorId = static_cast<Uint>(desc.VendorId);

			setOutputVars(pAdapter, adp->deviceName, adp->monitorHandle);

			adp->supportedDisplayModes = getSupportedDisplayModes(pAdapter);

			if (adp->supportedDisplayModes && adp->supportedDisplayModes->Count() > 0) {
				setCurrentDisplayMode(*adp->supportedDisplayModes, SurfaceFormat::Color,
					GraphicsDeviceManager::DefaultBackBufferWidth,
					GraphicsDeviceManager::DefaultBackBufferHeight, adp->currentDisplayMode);
			}

			return adp;
		}

		return nullptr;
	}

	void GraphicsAdapter::Adapters(std::vector<uptr<GraphicsAdapter>>& adapters) {
		comptr<IDXGIFactory1> pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.GetAddressOf()))
			Exception::Throw(Exception::FAILED_TO_CREATE);

		comptr<IDXGIAdapter1> pAdapter = nullptr;

		for (UINT count = 0; pFactory->EnumAdapters1(count, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = uptr<GraphicsAdapter>(new GraphicsAdapter());

			adp->impl->adapter = pAdapter;
			adp->impl->factory = pFactory;

			DXGI_ADAPTER_DESC1 desc{};
			pAdapter->GetDesc1(&desc);

			adp->description = XnaHelper::ToString(desc.Description);
			adp->deviceId = static_cast<Uint>(desc.DeviceId);			
			adp->isDefault = count == 0;
			adp->revision = static_cast<Uint>(desc.Revision);
			adp->subSystemId = static_cast<Uint>(desc.SubSysId);
			adp->vendorId = static_cast<Uint>(desc.VendorId);

			setOutputVars(pAdapter, adp->deviceName, adp->monitorHandle);

			adp->supportedDisplayModes = getSupportedDisplayModes(pAdapter);

			if (adp->supportedDisplayModes && adp->supportedDisplayModes->Count() > 0) {
				setCurrentDisplayMode(*adp->supportedDisplayModes, SurfaceFormat::Color,
					GraphicsDeviceManager::DefaultBackBufferWidth,
					GraphicsDeviceManager::DefaultBackBufferHeight, adp->currentDisplayMode);
			}						

			adapters.push_back(std::move(adp));
		}
	}

	bool GraphicsAdapter::QueryBackBufferFormat(
		GraphicsProfile graphicsProfile, SurfaceFormat format,
		DepthFormat depthFormat, Int multiSampleCount,
		SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat,
		Int& selectedMultiSampleCount) const 
	{
		selectedFormat = format;
		selectedDepthFormat = depthFormat;
		selectedMultiSampleCount = multiSampleCount;

		comptr<IDXGIOutput> pOutput = nullptr;		

		if (impl->adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND){
			comptr<IDXGIOutput1> pOutput1 = nullptr;

			pOutput->QueryInterface(IID_IDXGIOutput1, (void**)pOutput1.GetAddressOf());

			DXGI_MODE_DESC1 modeToMath{};
			modeToMath.Format = DxHelpers::SurfaceFormatToDx(format);

			//If pConcernedDevice is NULL, the Format member of DXGI_MODE_DESC1 cannot be DXGI_FORMAT_UNKNOWN.
			if (modeToMath.Format == DXGI_FORMAT_UNKNOWN)
				return false;

			DXGI_MODE_DESC1 closestMath;			
			const auto hresult = pOutput1->FindClosestMatchingMode1(&modeToMath, &closestMath, nullptr);

			if FAILED(hresult)
				return false;

			selectedFormat = DxHelpers::SurfaceFormatToXna(closestMath.Format);

			return selectedFormat == format;
		}

		return false;
	}

	//INTERNAL FUNCTIONS

	sptr<DisplayModeCollection> getSupportedDisplayModes(comptr<IDXGIAdapter1>& dxAdapter) {
		const auto totalDisplay = getDisplayModesCount(dxAdapter.Get());

		if (totalDisplay == 0)
			return nullptr;

		comptr<IDXGIOutput> pOutput = nullptr;
		comptr<IDXGIOutput1> pOutput1 = nullptr;
		UINT bufferOffset = 0;

		std::vector<DXGI_MODE_DESC1> buffer(totalDisplay);

		if (dxAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT numModes = 0;

				if (!pOutput1) {
					pOutput->QueryInterface(IID_IDXGIOutput1, (void**)pOutput1.GetAddressOf());
				}

				//See ref: https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgioutput-getdisplaymodelist?redirectedfrom=MSDN
				pOutput1->GetDisplayModeList1(format, 0, &numModes, nullptr);

				if (numModes == 0)
					continue;

				pOutput1->GetDisplayModeList1(format, 0, &numModes, buffer.data() + bufferOffset);

				bufferOffset += numModes;
			}
		}

		if (!pOutput)
			return nullptr;

		return createDisplayModeCollection(buffer);
	}		

	void setCurrentDisplayMode(DisplayModeCollection& displayModes, SurfaceFormat surfaceFormat, Uint width, Uint height, sptr<DisplayMode>& currentDisplayMode) {		
		auto modes = displayModes.Query(surfaceFormat);

		for (size_t i = 0; i < modes.size(); ++i) {
			auto& m = modes[i];

			if (m->Format() == surfaceFormat && m->Width() == width && m->Height() == height) {
				currentDisplayMode = m;
			}
			else if (i + 1 == modes.size()) {
				currentDisplayMode = m;
			}
		}
	}

	size_t getDisplayModesCount(IDXGIAdapter* adapter) {
		comptr<IDXGIOutput> pOutput = nullptr;		
		comptr<IDXGIOutput1> pOutput1 = nullptr;		
		size_t numModes = 0;		

		
		if (adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT num = 0;

				if (!pOutput1) {					
					pOutput->QueryInterface(IID_IDXGIOutput1, (void**)pOutput1.GetAddressOf());
				}

				//See ref: https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgioutput-getdisplaymodelist?redirectedfrom=MSDN
				pOutput1->GetDisplayModeList1(format, 0, &num, nullptr);

				numModes += num;
			}
		}

		return numModes;
	}

	sptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC1> const& source) {
		auto collection = snew<DisplayModeCollection>();

		std::vector<sptr<DisplayMode>> displayList;
		sptr<DisplayMode> pDisplay = nullptr;

		for (size_t i = 0; i < source.size(); ++i) {
			auto& modedesc = source[i];

			DisplayModeRate rate;
			rate.RefreshRate.Denominator = modedesc.RefreshRate.Denominator;
			rate.RefreshRate.Numerator = modedesc.RefreshRate.Numerator;
			rate.Scaling = static_cast<DisplayModeScaling>(modedesc.Scaling);
			rate.ScanlineOrdering = static_cast<DisplayModeScanlineOrder>(modedesc.ScanlineOrdering);

			if (pDisplay && pDisplay->Width() == modedesc.Width && pDisplay->Height() == modedesc.Height && pDisplay->Format() == DxHelpers::SurfaceFormatToXna(modedesc.Format)) {
				pDisplay->Rates.push_back(rate);
			}
			else {
				pDisplay = snew<DisplayMode>(
					modedesc.Width,
					modedesc.Height,
					DxHelpers::SurfaceFormatToXna(modedesc.Format));

				pDisplay->Rates.push_back(rate);
				displayList.push_back(pDisplay);
			}
		}

		collection->DisplayModes = displayList;

		return collection;
	}		

	static void setOutputVars(comptr<IDXGIAdapter1> const& adapter, String& deviceName, intptr_t& monitorHandle) {
		comptr<IDXGIOutput> pOutput = nullptr;

		if (adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC outputDesc;
			pOutput->GetDesc(&outputDesc);

			deviceName = XnaHelper::ToString(outputDesc.DeviceName);
			monitorHandle = reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}
	}
}