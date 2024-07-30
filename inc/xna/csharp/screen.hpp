#ifndef XNA_CSHARP_SCREEN_HPP
#define XNA_CSHARP_SCREEN_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"

namespace xna {
	//A simplified port of System.Windows.Forms.Screen.
	//Represents a display device or multiple display devices on a single system.
	class Screen {
	public:
		Screen(intptr_t hmonitor, bool primary, Rectangle bounds, Rectangle workingArea, String const& deviceName) :
			hmonitor(hmonitor), primary(primary), bounds(bounds), workingArea(workingArea), deviceName(deviceName) {}

		//Gets an array of all displays on the system.
		static std::vector<uptr<Screen>> AllScreens();

		//Gets the bounds of the display.
		constexpr Rectangle Bounds() const { return bounds; }
		
		//Gets the working area of the display.The working area is the desktop area of
		//the display, excluding taskbars, docked windows, and docked tool bars.
		constexpr Rectangle WorkingArea() const { return workingArea; }
		
		//Gets the monitor handler.
		constexpr intptr_t HMonitor() const { return hmonitor; }
		
		// Gets a value indicating whether a particular display is the primary device.
		constexpr bool Primary() const { return primary; }
		
		// Gets the device name associated with a display.
		constexpr String DeviceName() const { return deviceName; }

		constexpr bool operator==(Screen const& other) const {
			return hmonitor == other.hmonitor;
		}

	private:
		intptr_t hmonitor{ 0 };
		bool primary{ false };
		Rectangle bounds{};
		Rectangle workingArea{};
		String deviceName;
	};
}

#endif