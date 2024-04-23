#include "adapter-dx.hpp"
#include "../helpers.hpp"
#include "gdevicemanager-dx.hpp"

namespace xna {
	static size_t getDisplayModesCount(IDXGIAdapter* adapter);
	static UDisplayModeCollection createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source);

	UGraphicsAdapter IGraphicsAdapter::DefaultAdapter() {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return nullptr;

		IDXGIAdapter1* pAdapter = nullptr;
		
		if (pFactory->EnumAdapters1(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			auto adp = uNew<GraphicsAdapter>();

			adp->_index = 0;
			adp->dxadapter = pAdapter;

			return adp;
		}

		pFactory->Release();
		pFactory = nullptr;

		return nullptr;
	}

	void IGraphicsAdapter::Adapters(std::vector<PGraphicsAdapter>& adapters){
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;
		
		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = New<GraphicsAdapter>();

			adp->_index = count;
			adp->dxadapter = pAdapter;

			adapters.push_back(adp);
		}

		pFactory->Release();
		pFactory = nullptr;
	}

	void IGraphicsAdapter::Adapters(std::vector<UGraphicsAdapter>& adapters) {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;

		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = uNew<GraphicsAdapter>();

			adp->_index = count;
			adp->dxadapter = pAdapter;

			adapters.push_back(std::move(adp));
		}

		pFactory->Release();
		pFactory = nullptr;
	}

	String GraphicsAdapter::Description() const {
		if (!dxadapter) return String();

		DXGI_ADAPTER_DESC1 desc;
		dxadapter->GetDesc1(&desc);
		String description = XnaHToString(desc.Description);
		return description;
	}

	Uint GraphicsAdapter::DeviceId() const {
		if (!dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String GraphicsAdapter::DeviceName() const {
		if (!dxadapter) return String();

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;
		
		if (dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);
			String deviceName = XnaHToString(outputDesc.DeviceName);
			
			pOutput->Release();
			pOutput = nullptr;

			return deviceName;
		}
		
		return String();
	}

	intptr_t GraphicsAdapter::MonitorHandle() const {
		if (!dxadapter) return 0;

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;

		if (dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);			

			pOutput->Release();
			pOutput = nullptr;

			return reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}

		return 0;
	}

	Uint GraphicsAdapter::Revision() const {
		if (!dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint GraphicsAdapter::SubSystemId() const {
		if (!dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint GraphicsAdapter::VendorId() const {
		if (!dxadapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		dxadapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
	}	

	UDisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		if (!dxadapter) return nullptr;

		const auto totalDisplay = getDisplayModesCount(dxadapter);

		if (totalDisplay == 0)
			return nullptr;

		IDXGIOutput* pOutput = nullptr;		
		UINT bufferOffset = 0;
		
		std::vector<DXGI_MODE_DESC> buffer(totalDisplay);		
		
		if (dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = GraphicsAdapter::ToDXGI(currentSurface);

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

	UDisplayModeCollection GraphicsAdapter::SupportedDisplayModes(SurfaceFormat surfaceFormat) const
	{
		if (!dxadapter) return nullptr;				

		IDXGIOutput* pOutput = nullptr;
		UINT bufferOffset = 0;		

		if (dxadapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {			
			DXGI_FORMAT format = GraphicsAdapter::ToDXGI(surfaceFormat);

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

	PDisplayMode GraphicsAdapter::CurrentDisplayMode() {
		if (!_currentDisplayMode) {
			CurrentDisplayMode(SurfaceFormat::Color, GraphicsDeviceManager::DefaultBackBufferWidth, GraphicsDeviceManager::DefaultBackBufferHeight);
		}

		return _currentDisplayMode;
	}

	void GraphicsAdapter::CurrentDisplayMode(SurfaceFormat surfaceFormat, Uint width, Uint height) {
		const auto modes = SupportedDisplayModes(surfaceFormat);

		for (size_t i = 0; i < modes->_displayModes.size(); ++i) {
			auto& m = modes->_displayModes[i];
			
			if (m->_format == surfaceFormat && m->_width == width && m->_height == height) {
				_currentDisplayMode = m;
			}
			else if(i + 1 == modes->_displayModes.size()) {
				_currentDisplayMode = m;
			}
		}
	}

	bool GraphicsAdapter::GetOutput(UINT slot, IDXGIOutput*& output) {
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
				DXGI_FORMAT format = GraphicsAdapter::ToDXGI(currentSurface);

				UINT num = 0;
				pOutput->GetDisplayModeList(format, 0, &num, nullptr);

				numModes += num;
			}

			pOutput->Release();
			pOutput = nullptr;
		}

		return numModes;
	}

	static UDisplayModeCollection createDisplayModeCollection(std::vector<DXGI_MODE_DESC> const& source) {
		auto collection = uNew<DisplayModeCollection>();
		DisplayMode currentDisplayMode;
		std::vector<PDisplayMode> displayList;
		PDisplayMode pDisplay = nullptr;

		for (size_t i = 0; i < source.size(); ++i) {
			auto& modedesc = source[i];

			DisplayModeDescription description;
			description._refreshRate = modedesc.RefreshRate;
			description._scaling = static_cast<DisplayModeScaling>(modedesc.Scaling);
			description._scanlineOrdering = static_cast<DisplayModeScanlineOrder>(modedesc.ScanlineOrdering);

			if (pDisplay && pDisplay->_width == modedesc.Width && pDisplay->_height == modedesc.Height && pDisplay->_format == GraphicsAdapter::ToSurface(modedesc.Format)) {
				pDisplay->_descriptions.push_back(description);
			}
			else {
				pDisplay = New<DisplayMode>();
				pDisplay->_width = modedesc.Width;
				pDisplay->_height = modedesc.Height;
				pDisplay->_format = GraphicsAdapter::ToSurface(modedesc.Format);
				pDisplay->_descriptions.push_back(description);
				displayList.push_back(pDisplay);
			}
		}

		collection->_displayModes = displayList;

		return collection;
	}
}