#include "xna/graphics/adapter.hpp"
#include "xna/graphics/displaymode.hpp"
#include "xna/game/gdevicemanager.hpp"
#include "xna/xna-dx.hpp"

namespace xna {
	static size_t getDisplayModesCount(IDXGIAdapter* adapter);
	static uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source);

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

			adp->impl->_index = 0;
			adp->impl->dxAdapter = pAdapter;
			adp->impl->dxFactory = pFactory;

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

			adp->impl->_index = count;
			adp->impl->dxAdapter = pAdapter;
			adp->impl->dxFactory = pFactory;

			adapters.push_back(std::move(adp));
		}
	}	

	String GraphicsAdapter::Description() const {
		if (!impl->dxAdapter) return String();

		DXGI_ADAPTER_DESC1 desc;
		impl->dxAdapter->GetDesc1(&desc);
		String description = XnaHelper::ToString(desc.Description);
		return description;
	}

	Uint GraphicsAdapter::DeviceId() const {
		if (!impl->dxAdapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxAdapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String GraphicsAdapter::DeviceName() const {
		if (!impl->dxAdapter) return String();

		comptr<IDXGIOutput> pOutput = nullptr;		
		
		if (impl->dxAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC outputDesc{};
			pOutput->GetDesc(&outputDesc);

			String deviceName = XnaHelper::ToString(outputDesc.DeviceName);

			return deviceName;
		}
		
		return String();
	}

	intptr_t GraphicsAdapter::MonitorHandle() const {
		if (!impl->dxAdapter) return 0;

		comptr<IDXGIOutput> pOutput = nullptr;		

		if (impl->dxAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC outputDesc;
			pOutput->GetDesc(&outputDesc);						

			return reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}

		return 0;
	}

	Uint GraphicsAdapter::Revision() const {
		if (!impl->dxAdapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxAdapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint GraphicsAdapter::SubSystemId() const {
		if (!impl->dxAdapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxAdapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint GraphicsAdapter::VendorId() const {
		if (!impl->dxAdapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxAdapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
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
		if (!impl->_currentDisplayMode) {
			CurrentDisplayMode(SurfaceFormat::Color, GraphicsDeviceManager::DefaultBackBufferWidth, GraphicsDeviceManager::DefaultBackBufferHeight);
		}

		return impl->_currentDisplayMode;
	}

	void GraphicsAdapter::CurrentDisplayMode(SurfaceFormat surfaceFormat, Uint width, Uint height) {
		const auto modes = SupportedDisplayModes(surfaceFormat);

		for (size_t i = 0; i < modes->DisplayModes.size(); ++i) {
			auto& m = modes->DisplayModes[i];
			
			if (m->Format == surfaceFormat && m->Width == width && m->Height == height) {
				impl->_currentDisplayMode = m;
			}
			else if(i + 1 == modes->DisplayModes.size()) {
				impl->_currentDisplayMode = m;
			}
		}
	}

	bool GraphicsAdapter::PlatformImplementation::GetOutput(UINT slot, IDXGIOutput*& output) const {
		if (!dxAdapter) return false;

		if (dxAdapter->EnumOutputs(slot, &output) != DXGI_ERROR_NOT_FOUND)
			return true;

		return false;
	}

	//INTERNAL FUNCTIONS

	static size_t getDisplayModesCount(IDXGIAdapter* adapter) {
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

	static uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source) {
		auto collection = unew<DisplayModeCollection>();
		DisplayMode currentDisplayMode;
		std::vector<sptr<DisplayMode>> displayList;
		sptr<DisplayMode> pDisplay = nullptr;

		for (size_t i = 0; i < source.size(); ++i) {
			auto& modedesc = source[i];

			DisplayModeDescription description;
			description._refreshRate = modedesc.RefreshRate;
			description._scaling = static_cast<DisplayModeScaling>(modedesc.Scaling);
			description._scanlineOrdering = static_cast<DisplayModeScanlineOrder>(modedesc.ScanlineOrdering);

			if (pDisplay && pDisplay->Width == modedesc.Width && pDisplay->Height == modedesc.Height && pDisplay->Format == DxHelpers::SurfaceFormatToXna(modedesc.Format)) {
				pDisplay->impl->Descriptions.push_back(description);
			}
			else {
				pDisplay = snew<DisplayMode>();
				pDisplay->Width = modedesc.Width;
				pDisplay->Height = modedesc.Height;
				pDisplay->Format = DxHelpers::SurfaceFormatToXna(modedesc.Format);
				pDisplay->impl->Descriptions.push_back(description);
				displayList.push_back(pDisplay);
			}
		}

		collection->DisplayModes = displayList;

		return collection;
	}
}