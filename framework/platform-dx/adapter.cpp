#include "graphics/adapter.hpp"
#include "graphics/displaymode.hpp"
#include "platform-dx/headers.hpp"
#include "platform-dx/helpers.hpp"
#include "platform-dx/implementations.hpp"
#include "game/gdevicemanager.hpp"

namespace xna {
	static size_t getDisplayModesCount(IDXGIAdapter* adapter);
	static uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source);

	GraphicsAdapter::GraphicsAdapter() {
		impl = uNew<PlatformImplementation>();
	}

	GraphicsAdapter::~GraphicsAdapter() {
		impl = nullptr;
	}

	uptr<GraphicsAdapter> GraphicsAdapter::DefaultAdapter() {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return nullptr;

		IDXGIAdapter1* pAdapter = nullptr;
		
		if (pFactory->EnumAdapters1(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			auto adp = uNew<GraphicsAdapter>();

			adp->impl->_index = 0;
			adp->impl->dxadapter = pAdapter;

			return adp;
		}

		pFactory->Release();
		pFactory = nullptr;

		return nullptr;
	}

	void GraphicsAdapter::Adapters(std::vector<sptr<GraphicsAdapter>>& adapters){
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;
		
		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = New<GraphicsAdapter>();

			adp->impl->_index = count;
			adp->impl->dxadapter = pAdapter;

			adapters.push_back(adp);
		}

		pFactory->Release();
		pFactory = nullptr;
	}

	void GraphicsAdapter::Adapters(std::vector<uptr<GraphicsAdapter>>& adapters) {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;

		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = uNew<GraphicsAdapter>();

			adp->impl->_index = count;
			adp->impl->dxadapter = pAdapter;

			adapters.push_back(std::move(adp));
		}

		pFactory->Release();
		pFactory = nullptr;
	}	

	String GraphicsAdapter::Description() const {
		if (!impl->dxadapter) return String();

		DXGI_ADAPTER_DESC1 desc;
		impl->dxadapter->GetDesc1(&desc);
		String description = XnaHToString(desc.Description);
		return description;
	}

	Uint GraphicsAdapter::DeviceId() const {
		if (!impl->dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String GraphicsAdapter::DeviceName() const {
		if (!impl->dxadapter) return String();

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;
		
		if (impl->dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);
			String deviceName = XnaHToString(outputDesc.DeviceName);
			
			pOutput->Release();
			pOutput = nullptr;

			return deviceName;
		}
		
		return String();
	}

	intptr_t GraphicsAdapter::MonitorHandle() const {
		if (!impl->dxadapter) return 0;

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;

		if (impl->dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);			

			pOutput->Release();
			pOutput = nullptr;

			return reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}

		return 0;
	}

	Uint GraphicsAdapter::Revision() const {
		if (!impl->dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint GraphicsAdapter::SubSystemId() const {
		if (!impl->dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint GraphicsAdapter::VendorId() const {
		if (!impl->dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		impl->dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
	}	

	uptr<DisplayModeCollection> GraphicsAdapter::SupportedDisplayModes() const {
		if (!impl->dxadapter) return nullptr;

		const auto totalDisplay = getDisplayModesCount(impl->dxadapter);

		if (totalDisplay == 0)
			return nullptr;

		IDXGIOutput* pOutput = nullptr;		
		UINT bufferOffset = 0;
		
		std::vector<DXGI_MODE_DESC> buffer(totalDisplay);		
		
		if (impl->dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::ConvertSurfaceToDXGIFORMAT(currentSurface);

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

		pOutput->Release();
		pOutput = nullptr;

		return createDisplayModeCollection(buffer);
	}

	uptr<DisplayModeCollection> GraphicsAdapter::SupportedDisplayModes(SurfaceFormat surfaceFormat) const
	{
		if (!impl->dxadapter) return nullptr;				

		IDXGIOutput* pOutput = nullptr;
		UINT bufferOffset = 0;		

		if (impl->dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {			
			DXGI_FORMAT format = DxHelpers::ConvertSurfaceToDXGIFORMAT(surfaceFormat);

			UINT numModes = 0;

			pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);

			if (numModes == 0)
				return uNew<DisplayModeCollection>();

			std::vector<DXGI_MODE_DESC> buffer(numModes);
			pOutput->GetDisplayModeList(format, 0, &numModes, buffer.data());

			pOutput->Release();
			pOutput = nullptr;

			return createDisplayModeCollection(buffer);
		}		

		return uNew<DisplayModeCollection>();
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

	bool GraphicsAdapter::PlatformImplementation::GetOutput(UINT slot, IDXGIOutput*& output) {
		if (!dxadapter) return false;

		if (dxadapter->EnumOutputs(slot, &output) != DXGI_ERROR_NOT_FOUND)
			return true;

		return false;
	}

	//INTERNAL FUNCTIONS

	static size_t getDisplayModesCount(IDXGIAdapter* adapter) {
		IDXGIOutput* pOutput = nullptr;
		size_t numModes = 0;

		if (adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = DxHelpers::ConvertSurfaceToDXGIFORMAT(currentSurface);

				UINT num = 0;
				pOutput->GetDisplayModeList(format, 0, &num, nullptr);

				numModes += num;
			}

			pOutput->Release();
			pOutput = nullptr;
		}

		return numModes;
	}

	static uptr<DisplayModeCollection> createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source) {
		auto collection = uNew<DisplayModeCollection>();
		DisplayMode currentDisplayMode;
		std::vector<sptr<DisplayMode>> displayList;
		sptr<DisplayMode> pDisplay = nullptr;

		for (size_t i = 0; i < source.size(); ++i) {
			auto& modedesc = source[i];

			DisplayModeDescription description;
			description._refreshRate = modedesc.RefreshRate;
			description._scaling = static_cast<DisplayModeScaling>(modedesc.Scaling);
			description._scanlineOrdering = static_cast<DisplayModeScanlineOrder>(modedesc.ScanlineOrdering);

			if (pDisplay && pDisplay->Width == modedesc.Width && pDisplay->Height == modedesc.Height && pDisplay->Format == DxHelpers::ConvertDXGIFORMATToSurface(modedesc.Format)) {
				pDisplay->impl->Descriptions.push_back(description);
			}
			else {
				pDisplay = New<DisplayMode>();
				pDisplay->Width = modedesc.Width;
				pDisplay->Height = modedesc.Height;
				pDisplay->Format = DxHelpers::ConvertDXGIFORMATToSurface(modedesc.Format);
				pDisplay->impl->Descriptions.push_back(description);
				displayList.push_back(pDisplay);
			}
		}

		collection->DisplayModes = displayList;

		return collection;
	}
}