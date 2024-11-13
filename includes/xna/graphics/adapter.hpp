#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../platform.hpp"
#include "displaymode.hpp"
#include <cstdint>
#include <memory>
#include <string>

namespace xna {
	
	struct GraphicsAdapterImplementation;

	//Provides methods to retrieve and manipulate graphics adapters.
	class GraphicsAdapter : public PlatformImplementation<GraphicsAdapterImplementation> {
	public:
		//Collection of available adapters on the system.
		static void Adapters(std::vector<std::unique_ptr<GraphicsAdapter>>& adapters);

		//Gets the current display mode.
		inline std::shared_ptr<DisplayMode> CurrentDisplayMode() const { return currentDisplayMode; }

		//Gets the default adapter. 
		static std::unique_ptr<GraphicsAdapter> DefaultAdapter();

		//Retrieves a string used for presentation to the user.
		constexpr std::string Description() const { return description; }

		//Retrieves a value that is used to help identify a particular chip set. 
		constexpr uint32_t DeviceId() const { return deviceId; }
		
		//Retrieves a string that contains the device name.
		constexpr std::string DeviceName() const { return deviceName; }
		
		//Determines if this instance of GraphicsAdapter is the default adapter.
		constexpr bool IsDefaultAdapter() const { return isDefault; }
		
		//Determines if the graphics adapter is in widescreen mode.
		inline bool IsWideScreen() const { 
			return currentDisplayMode->AspectRatio() > 1.6000000238418579;
		}
		
		//Retrieves the handle of the monitor
		constexpr intptr_t MonitorHandle() const { return monitorHandle; }

		//Retrieves a value used to help identify the revision level of a particular chip set.
		constexpr uint32_t Revision() const { return revision; }
		
		//Retrieves a value used to identify the subsystem.
		constexpr uint32_t SubSystemId() const { return subSystemId; }
		
		//Returns a collection of supported display modes for the current adapter.
		inline std::shared_ptr<DisplayModeCollection> SupportedDisplayModes() const { return supportedDisplayModes; }

		//Retrieves a value used to identify the manufacturer.
		constexpr uint32_t VendorId() const { return vendorId; }
		
		//Gets or sets a NULL device. 
		static bool UseNullDevice() { return useNullDevice; }
		//Gets or sets a NULL device. 
		static void UseNullDevice(bool value) { useNullDevice = value; }
		
		//Gets or sets a reference device.
		constexpr static bool UseReferenceDevice() { return useReferenceDevice; }
		//Gets or sets a reference device. 
		constexpr static void UseReferenceDevice(bool value) { useReferenceDevice = value; }

		//Tests to see if the adapter supports the requested profile. 
		bool IsProfileSupported(GraphicsProfile graphicsProfile) {
			return true;
		}		

		//Queries the adapter for support for the requested back buffer format.
		bool QueryBackBufferFormat(
			GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			int32_t multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int32_t& selectedMultiSampleCount
		) const;		

	private:
		std::string description;
		uint32_t deviceId{0};
		std::string deviceName;
		bool isDefault{ false };
		intptr_t monitorHandle{ 0 };
		uint32_t revision{ 0 };
		uint32_t subSystemId{ 0 };
		uint32_t vendorId{ 0 };
		std::shared_ptr<DisplayMode> currentDisplayMode{ nullptr };
		std::shared_ptr<DisplayModeCollection> supportedDisplayModes{ nullptr };

		inline static bool useNullDevice = false;
		inline static bool useReferenceDevice = false;

		GraphicsAdapter();	
	};
}

#endif