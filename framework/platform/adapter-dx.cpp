#include "adapter-dx.hpp"
#include "../helpers.hpp"

namespace xna {

	String GraphicsAdapter::Description() const {
		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);
		String description = WStringToString(desc.Description);
		return description;
	}

	Uint GraphicsAdapter::DeviceId() const {
		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.DeviceId);
	}

	String GraphicsAdapter::DeviceName() const {
		IDXGIOutput* pOutput = nullptr;
		DXGI_OUTPUT_DESC outputDesc;
		
		if (_adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			pOutput->GetDesc(&outputDesc);
			String deviceName = WStringToString(outputDesc.DeviceName);
			
			pOutput->Release();
			pOutput = nullptr;

			return deviceName;
		}
		
		return String();
	}

	intptr_t GraphicsAdapter::MonitorHandle() const {
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
		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.Revision);
	}

	Uint GraphicsAdapter::SubSystemId() const {
		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.SubSysId);
	}

	Uint GraphicsAdapter::VendorId() const {
		DXGI_ADAPTER_DESC1 desc;
		_adapter->GetDesc1(&desc);

		return static_cast<Uint>(desc.VendorId);
	}

	PDisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		IDXGIOutput* pOutput = nullptr;
		UINT numModes = 0;
		UINT totalModes = 0;
		std::vector<DXGI_MODE_DESC> buffer(250);
		
		if (_adapter->EnumOutputs(0, &pOutput) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < SURFACE_FORMAT_COUNT; ++f) {
				const auto currentSurface = static_cast<SurfaceFormat>(f);
				DXGI_FORMAT format = SurfaceFormatMapper::ParseToDXGI(currentSurface);

				pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);

				if (numModes == 0)
					continue;

				if (buffer.size() < static_cast<size_t>(totalModes) + numModes)
					buffer.resize(static_cast<size_t>(totalModes * 2));

				pOutput->GetDisplayModeList(format, 0, &numModes, buffer.data() + totalModes);

				totalModes += numModes;
			}

			pOutput->Release();						

			buffer.resize(totalModes);
			std::vector<DisplayMode> dmodes;

			for (size_t i = 0; i < buffer.size(); ++i) {
				const auto& modedesc = buffer[i];
				const auto surface = SurfaceFormatMapper::ParseToSurface(modedesc.Format);

				dmodes[0] = DisplayMode(modedesc.Width, modedesc.Height, surface);
			}

			dmodes.resize(buffer.size());
			return New<DisplayModeCollection>(dmodes);
		}

		return New<DisplayModeCollection>();
	}

	std::vector<PGraphicsAdapter> IGraphicsAdapter::getAllAdapters() {
		IDXGIFactory1* pFactory = nullptr;
		std::vector<PGraphicsAdapter> adapters;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)) {
			return adapters;
		}

		IDXGIAdapter1* pAdapter = nullptr;
		UINT count = 0;

		for (; pFactory->EnumAdapters1(count, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++count) {
			auto adp = New<GraphicsAdapter>();
			
			adp->_index = count;
			adp->_adapter = pAdapter;

			adapters.push_back(adp);
		}

		if (!adapters.empty())
			adapters.resize(count);

		return adapters;
	}
}