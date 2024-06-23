#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../default.hpp"

namespace xna {
	//Provides methods to retrieve and manipulate graphics adapters.
	class GraphicsAdapter {
	public:
		GraphicsAdapter();

		//Retrieves a string used for presentation to the user.
		String Description() const;
		//Retrieves a value that is used to help identify a particular chip set. 
		Uint DeviceId() const;
		//Retrieves a string that contains the device name.
		String DeviceName() const;
		//Determines if this instance of GraphicsAdapter is the default adapter.
		bool IsDefaultAdapter() const;
		//Retrieves the handle of the monitor
		intptr_t MonitorHandle() const;
		//Retrieves a value used to help identify the revision level of a particular chip set.
		Uint Revision() const;
		//Retrieves a value used to identify the subsystem.
		Uint SubSystemId() const;
		//Retrieves a value used to identify the manufacturer.
		Uint VendorId() const;

		uptr<DisplayModeCollection> SupportedDisplayModes() const;
		uptr<DisplayModeCollection> SupportedDisplayModes(SurfaceFormat surfaceFormat) const;
		
		//Gets the current display mode.
		sptr<DisplayMode> CurrentDisplayMode();

		//Gets the current display mode.
		void CurrentDisplayMode(SurfaceFormat surfaceFormat, Uint width, Uint height);

		//Gets the default adapter. 
		static uptr<GraphicsAdapter> DefaultAdapter();
		
		//Collection of available adapters on the system.
		static void Adapters(std::vector<uptr<GraphicsAdapter>>& adapters);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif