#include "adapter-dx.hpp"
#include "../helpers.hpp"

namespace xna {

	UGraphicsAdapter IGraphicsAdapter::DefaultAdapter() {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return nullptr;

		IDXGIAdapter1* pAdapter = nullptr;
		
		if (pFactory->EnumAdapters1(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			auto adp = uNew<GraphicsAdapter>();

			adp->_index = 0;
			adp->_adapter = pAdapter;

			return std::move(adp);
		}

		pFactory->Release();
		pFactory = nullptr;

		return nullptr;
	}

	void IGraphicsAdapter::GetAllAdapters(std::vector<PGraphicsAdapter>& adapters){
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;
		
		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = New<GraphicsAdapter>();

			adp->_index = count;
			adp->_adapter = pAdapter;

			adapters.push_back(adp);
		}

		pFactory->Release();
		pFactory = nullptr;
	}

	void IGraphicsAdapter::GetAllAdapters(std::vector<UGraphicsAdapter>& adapters) {
		IDXGIFactory1* pFactory = nullptr;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory))
			return;

		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = uNew<GraphicsAdapter>();

			adp->_index = count;
			adp->_adapter = pAdapter;

			adapters.push_back(std::move(adp));
		}

		pFactory->Release();
		pFactory = nullptr;
	}

	String GraphicsAdapter::Description() const {
		if (!_adapter) return String();

		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);
		String description = XnaHToString(desc.Description);
		return description;
	}

	Uint GraphicsAdapter::DeviceId() const {
		if (!_adapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String GraphicsAdapter::DeviceName() const {
		if (!_adapter) return String();

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;
		
		if (_adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);
			String deviceName = XnaHToString(outputDesc.DeviceName);
			
			pOutput->Release();
			pOutput = nullptr;

			return deviceName;
		}
		
		return String();
	}

	intptr_t GraphicsAdapter::MonitorHandle() const {
		if (!_adapter) return 0;

		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;

		if (_adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);			

			pOutput->Release();
			pOutput = nullptr;

			return reinterpret_cast<intptr_t>(outputDesc.Monitor);
		}

		return 0;
	}

	Uint GraphicsAdapter::Revision() const {
		if (!_adapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint GraphicsAdapter::SubSystemId() const {
		if (!_adapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint GraphicsAdapter::VendorId() const {
		if (!_adapter) return 0;

		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
	}

	UDisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		if (!_adapter) return nullptr;

		IDXGIOutput* pOutput = nullptr;
		UINT numModes = 0;
		UINT totalModes = 0;
		std::vector<DXGI_MODE_DESC> buffer(500);
		
		if (_adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = GraphicsAdapter::ToDXGI(currentSurface);

				pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);

				if (numModes == 0)
					continue;

				if (buffer.size() < static_cast<size_t>(totalModes) + numModes)
					buffer.resize(static_cast<size_t>(totalModes * 2));

				pOutput->GetDisplayModeList(format, 0, &numModes, buffer.data() + totalModes);

				totalModes += numModes;
			}

			pOutput->Release();			
			
			auto collection = uNew<DisplayModeCollection>(totalModes);

			for (size_t i = 0; i < totalModes; ++i) {
				const auto& modedesc = buffer[i];
				const auto surface = GraphicsAdapter::ToSurface(modedesc.Format);

				collection->_displayModes[i] = DisplayMode(modedesc.Width, modedesc.Height, surface);
			}
			
			return std::move(collection);
		}

		return nullptr;
	}	
}