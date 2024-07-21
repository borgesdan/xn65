#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../default.hpp"
#include "displaymode.hpp"

namespace xna {
	//Provides methods to retrieve and manipulate graphics adapters.
	class GraphicsAdapter {
	public:
		//Collection of available adapters on the system.
		static void Adapters(std::vector<uptr<GraphicsAdapter>>& adapters);

		//Gets the current display mode.
		inline sptr<DisplayMode> CurrentDisplayMode() const { return currentDisplayMode; }

		//Gets the default adapter. 
		static uptr<GraphicsAdapter> DefaultAdapter();

		//Retrieves a string used for presentation to the user.
		constexpr String Description() const { return description; }

		//Retrieves a value that is used to help identify a particular chip set. 
		constexpr Uint DeviceId() const { return deviceId; }
		
		//Retrieves a string that contains the device name.
		constexpr String DeviceName() const { return deviceName; }
		
		//Determines if this instance of GraphicsAdapter is the default adapter.
		constexpr bool IsDefaultAdapter() const { return isDefault; }
		
		//Determines if the graphics adapter is in widescreen mode.
		inline bool IsWideScreen() const { 
			return currentDisplayMode->AspectRatio() > 1.6000000238418579;
		}
		
		//Retrieves the handle of the monitor
		constexpr intptr_t MonitorHandle() const { return monitorHandle; }

		//Retrieves a value used to help identify the revision level of a particular chip set.
		constexpr Uint Revision() const { return revision; }
		
		//Retrieves a value used to identify the subsystem.
		constexpr Uint SubSystemId() const { return subSystemId; }
		
		//Returns a collection of supported display modes for the current adapter.
		inline sptr<DisplayModeCollection> SupportedDisplayModes() const { return supportedDisplayModes; }

		//Retrieves a value used to identify the manufacturer.
		constexpr Uint VendorId() const { return vendorId; }

		//Tests to see if the adapter supports the requested profile. 
		bool IsProfileSupported(GraphicsProfile graphicsProfile) {
			return false;
		}

		//Queries the adapter for support for the requested back buffer format.
		bool QueryBackBufferFormat(
			GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			Int multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			Int& selectedMultiSampleCount
		) {
			return false;
		}

		//Queries the adapter for support for the requested render target format. 
		bool QueryRenderTargetFormat(
			GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			Int multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int& selectedMultiSampleCount
		) {
			return false;
		}

	private:
		String description;
		Uint deviceId{0};
		String deviceName;
		bool isDefault{ false };
		intptr_t monitorHandle{ 0 };
		Uint revision{ 0 };
		Uint subSystemId{ 0 };
		Uint vendorId{ 0 };
		sptr<DisplayMode> currentDisplayMode{ nullptr };
		sptr<DisplayModeCollection> supportedDisplayModes{ nullptr };

		GraphicsAdapter();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif