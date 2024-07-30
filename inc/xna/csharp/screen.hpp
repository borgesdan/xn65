#ifndef XNA_CSHARP_SCREEN_HPP
#define XNA_CSHARP_SCREEN_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"

namespace xna {
	class Screen {
	public:
		Screen(intptr_t hmonitor, bool primary, Rectangle bounds, Rectangle workingArea, String const& deviceName) : 
			hmonitor(hmonitor), primary(primary), bounds(bounds), workingArea(workingArea), deviceName(deviceName){}

		static std::vector<uptr<Screen>> AllScreens();

		constexpr Rectangle Bounds() const { return bounds; }
		constexpr Rectangle WorkingArea() const { return workingArea; }
		constexpr intptr_t HMonitor() const { return hmonitor; }
		constexpr bool Primary() const { return primary; }
		constexpr String DeviceName() const { return deviceName; }

	private:
		intptr_t hmonitor{ 0 };
		bool primary{ false };
		Rectangle bounds{};
		Rectangle workingArea{};
		String deviceName;
	};	
}

#endif