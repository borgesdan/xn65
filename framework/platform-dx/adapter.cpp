#include "xna/graphics/adapter.hpp"
#include "xna/graphics/displaymode.hpp"
#include "xna/game/gdevicemanager.hpp"
#include "xna/xna-dx.hpp"

namespace xna {
	static String getDescription(comptr<IDXGIAdapter1> const& adapter);
	static Uint getDeviceId(comptr<IDXGIAdapter1> const& adapter);
	static String getDeviceName(comptr<IDXGIAdapter1> const& adapter);
	static intptr_t getMonitorHandle(comptr<IDXGIAdapter1> const& adapter);
	static Uint getRevision(comptr<IDXGIAdapter1> const& adapter);
	static Uint getRevision(comptr<IDXGIAdapter1> const& adapter);
	static Uint getSubSystemId(comptr<IDXGIAdapter1> const& adapter);
	static Uint getVendorId(comptr<IDXGIAdapter1> const& adapter);
	static size_t getDisplayModesCount(IDXGIAdapter* adapter);
	static uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source);	
	static void setCurrentDisplayMode(GraphicsAdapter& adapter, SurfaceFormat surfaceFormat, Uint width, Uint height, sptr<DisplayMode>& currentDisplayMode);

	GraphicsAdapter::GraphicsAdapter() {
		impl = unew<PlatformImplementation>();
	}

	uptr<GraphicsAdapter> GraphicsAdapter::DefaultAdapter() {
		comptr<IDXGIFactory1> pFactory = nullptr;
		
		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.GetAddressOf()))
			Exception::Throw(Exception::FAILED_TO_CREATE);

		comptr<IDXGIAdapter1> pAdapter = nullptr;
		
		if (pFactory->EnumAdapters1(0, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			auto adp = unew<GraphicsAdapter>();
			
			adp->impl->dxAdapter = pAdapter;
			adp->impl->dxFactory = pFactory;			
			
			adp->description = getDescription(pAdapter);
			adp->deviceId = getDeviceId(pAdapter);
			adp->deviceName = getDeviceName(pAdapter);
			adp->isDefault = true;
			adp->monitorHandle = getMonitorHandle(pAdapter);
			adp->revision = getRevision(pAdapter);
			adp->subSystemId = getSubSystemId(pAdapter);
			adp->vendorId = getVendorId(pAdapter);			

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
			auto adp = unew<GraphicsAdapter>();

			adp->impl->dxAdapter = pAdapter;
			adp->impl->dxFactory = pFactory;

			adp->description = getDescription(pAdapter);
			adp->deviceId = getDeviceId(pAdapter);
			adp->deviceName = getDeviceName(pAdapter);
			adp->isDefault = count == 0;
			adp->monitorHandle = getMonitorHandle(pAdapter);
			adp->revision = getRevision(pAdapter);
			adp->subSystemId = getSubSystemId(pAdapter);
			adp->vendorId = getVendorId(pAdapter);

			adapters.push_back(std::move(adp));
		}
	}			

	uptr<DisplayModeCollection> GraphicsAdapter::SupportedDisplayModes() const {
		if (!impl->dxAdapter) return nullptr;

		const auto totalDisplay = getDisplayModesCount(impl->dxAdapter.Get());

		if (totalDisplay == 0)
			return nullptr;

		comptr<IDXGIOutput> pOutput = nullptr;
		UINT bufferOffset = 0;
		
		std::vector<DXGI_MODE_DESC> buffer(totalDisplay);		
		
		if (impl->dxAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT numModes = 0;
				pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);

				if (numModes == 0)
					continue;				

				pOutput->GetDisplayModeList(format, 0, &numModes, buffer.data() + bufferOffset);

				bufferOffset += numModes;
			}			
		}

		if (!pOutput)
			return nullptr;

		return createDisplayModeCollection(buffer);
	}

	uptr<DisplayModeCollection> GraphicsAdapter::SupportedDisplayModes(SurfaceFormat surfaceFormat) const
	{
		if (!impl->dxAdapter) return nullptr;				

		comptr<IDXGIOutput> pOutput = nullptr;
		UINT bufferOffset = 0;		

		if (impl->dxAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {			
			DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(surfaceFormat);

			UINT numModes = 0;

			pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);

			if (numModes == 0)
				return unew<DisplayModeCollection>();

			std::vector<DXGI_MODE_DESC> buffer(numModes);
			pOutput->GetDisplayModeList(format, 0, &numModes, buffer.data());			

			return createDisplayModeCollection(buffer);
		}		

		return unew<DisplayModeCollection>();
	}

	sptr<DisplayMode> GraphicsAdapter::CurrentDisplayMode() {
		if (!currentDisplayMode) {
			setCurrentDisplayMode(*this, 
				SurfaceFormat::Color, 
				GraphicsDeviceManager::DefaultBackBufferWidth,
				GraphicsDeviceManager::DefaultBackBufferHeight,
				currentDisplayMode);
		}

		return currentDisplayMode;
	}	

	bool GraphicsAdapter::PlatformImplementation::GetOutput(UINT slot, IDXGIOutput*& output) const {
		if (!dxAdapter) return false;

		if (dxAdapter->EnumOutputs(slot, &output) != DXGI_ERROR_NOT_FOUND)
			return true;

		return false;
	}

	//INTERNAL FUNCTIONS

	void setCurrentDisplayMode(GraphicsAdapter& adapter, SurfaceFormat surfaceFormat, Uint width, Uint height, sptr<DisplayMode>& currentDisplayMode) {
		const auto modes = adapter.SupportedDisplayModes(surfaceFormat);

		for (size_t i = 0; i < modes->DisplayModes.size(); ++i) {
			auto& m = modes->DisplayModes[i];

			if (m->Format() == surfaceFormat && m->Width() == width && m->Height() == height) {
				currentDisplayMode = m;
			}
			else if (i + 1 == modes->DisplayModes.size()) {
				currentDisplayMode = m;
			}
		}
	}

	size_t getDisplayModesCount(IDXGIAdapter* adapter) {
		comptr<IDXGIOutput> pOutput = nullptr;
		size_t numModes = 0;

		if (adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT num = 0;
				pOutput->GetDisplayModeList(format, 0, &num, nullptr);

				numModes += num;
			}			
		}

		return numModes;
	}

	uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source) {
		auto collection = unew<DisplayModeCollection>();
		
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

	String getDescription(comptr<IDXGIAdapter1> const& adapter) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		const auto description = XnaHelper::ToString(desc.Description);
		return description;
	}

	Uint getDeviceId(comptr<IDXGIAdapter1> const& adapter) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String getDeviceName(comptr<IDXGIAdapter1> const& adapter) {
		comptr<IDXGIOutput> pOutput = nullptr;

		if (adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC outputDesc{};
			pOutput->GetDesc(&outputDesc);

			String deviceName = XnaHelper::ToString(outputDesc.DeviceName);

			return deviceName;
		}

		return String();
	}

	intptr_t getMonitorHandle(comptr<IDXGIAdapter1> const& adapter) {
		comptr<IDXGIOutput> pOutput = nullptr;

		if (adapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC outputDesc;
			pOutput->GetDesc(&outputDesc);

			return reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}

		return 0;
	}

	Uint getRevision(comptr<IDXGIAdapter1> const& adapter) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint getSubSystemId(comptr<IDXGIAdapter1> const& adapter) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint getVendorId(comptr<IDXGIAdapter1> const& adapter) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
	}
}