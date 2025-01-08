#include "csharp/windows/forms/screen.hpp"
#include "csharp/windows/forms/system.hpp"
#include <limits>

#ifdef WIN32
#include "Windows.h"

namespace csharp {
	int Screen::s_primaryMonitor = 65537;

	Screen::Screen(intptr_t monitor, intptr_t hdc) {
		HDC screenDC = reinterpret_cast<HDC>(hdc);

		if (!SystemInformation::MultiMonitorSupport() || monitor == s_primaryMonitor) {
			_bounds = SystemInformation::VirtualScreen();
			_primary = true;
			_deviceName = "DISPLAY";
		}
		else {
			MONITORINFOEX info{};
			info.cbSize = sizeof(MONITORINFOEX);

			auto hmonitor = reinterpret_cast<HMONITOR>(monitor);
			GetMonitorInfo(hmonitor, &info);

			_bounds = Rectangle::FromLTRB(
				info.rcMonitor.left,
				info.rcMonitor.top,
				info.rcMonitor.right,
				info.rcMonitor.bottom);

			_primary = ((info.dwFlags & MONITORINFOF_PRIMARY) != 0);
			
			auto _deviceName = info.szDevice;

			if (hdc == 0) {
				screenDC = CreateDCW(LPCWSTR(_deviceName), NULL, NULL, NULL);
			}
		}

		_hmonitor = monitor;

		_bitDepth = GetDeviceCaps(screenDC, BITSPIXEL);
		_bitDepth *= GetDeviceCaps(screenDC, PLANES);

		if (hdc != reinterpret_cast<intptr_t>(screenDC)) {
			DeleteDC(screenDC);
		}
	}

	static BOOL MonitorEnumProcCallback(
		HMONITOR hmonitor,
		HDC hdc,
		LPRECT lprect,
		LPARAM lparam
	) {
		auto screens = (std::vector<std::unique_ptr<Screen>>*)lparam;
		auto screen = std::make_unique<Screen>(
			reinterpret_cast<intptr_t>(hmonitor), 
			reinterpret_cast<intptr_t>(hdc));

		screens->push_back(std::move(screen));

		return true;
	}

	std::vector<std::unique_ptr<Screen>> Screen::AllScreens() {
		std::vector<std::unique_ptr<Screen>> s_screens;

		if (SystemInformation::MultiMonitorSupport()) {
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProcCallback, (LPARAM)&s_screens);

			if (s_screens.empty()) {
				auto primary = std::make_unique<Screen>(s_primaryMonitor);
				s_screens.push_back(std::move(primary));
			}
		}
		else {
			auto primary = std::make_unique<Screen>(s_primaryMonitor);
			s_screens.push_back(std::move(primary));
		}

		return s_screens;
	}

	std::unique_ptr<Screen> Screen::PrimaryScreen() {
		if (SystemInformation::MultiMonitorSupport())
		{
			auto screens = AllScreens();
			for (size_t i = 0; i < screens.size(); ++i)
			{
				if (screens[i]->_primary)
				{
					return std::move(screens[i]);
				}
			}

			return nullptr;
		}
		else
		{
			return std::make_unique<Screen>(s_primaryMonitor, 0);
		}
	}
}

#endif