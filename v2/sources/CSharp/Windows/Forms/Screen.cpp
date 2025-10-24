#include "CSharp/Windows/Forms/Screen.hpp"
#include "CSharp/Windows/Forms/SystemInformation.hpp"
#include "../../../WindowsPlatform.hpp"

namespace CSharp {
	int Screen::s_primaryMonitor = 65537;

	static MONITORINFOEX getMonitorInfo(intptr_t monitor) {
		MONITORINFOEX info{};
		info.cbSize = sizeof(MONITORINFOEX);
		auto hmonitor = reinterpret_cast<HMONITOR>(monitor);
		GetMonitorInfo(hmonitor, &info);
		return info;
	}

	Screen::Screen(intptr_t monitor, intptr_t hdc) {
		HDC screenDC = reinterpret_cast<HDC>(hdc);

		if (!SystemInformation::MultiMonitorSupport() || monitor == s_primaryMonitor) {
			_bounds = SystemInformation::VirtualScreen();
			_primary = true;
			//_deviceName = "DISPLAY";			
			MONITORINFOEX info = getMonitorInfo(monitor);
			_deviceName = std::string(info.szDevice);
		}
		else {
			MONITORINFOEX info = getMonitorInfo(monitor);

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

	static BOOL MonitorEnumProcCallback(HMONITOR hmonitor, HDC hdc, LPRECT lprect, LPARAM lparam) {
		auto screens = (std::vector<Screen>*)lparam;
		auto screen = Screen(
			reinterpret_cast<intptr_t>(hmonitor),
			reinterpret_cast<intptr_t>(hdc));

		screens->push_back(screen);

		return true;
	}

	std::vector<Screen> Screen::AllScreens() {
		std::vector<Screen> s_screens;

		if (SystemInformation::MultiMonitorSupport()) {
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProcCallback, (LPARAM)&s_screens);

			if (s_screens.empty()) {
				auto primary = Screen(s_primaryMonitor);
				s_screens.push_back(primary);
			}
		}
		else {
			auto primary = Screen(s_primaryMonitor);
			s_screens.push_back(primary);
		}

		return s_screens;
	}

	Screen Screen::PrimaryScreen() {
		auto screen = Screen(s_primaryMonitor, 0);

		if (SystemInformation::MultiMonitorSupport())
		{
			auto screens = AllScreens();
			for (size_t i = 0; i < screens.size(); ++i)
			{
				if (screens[i]._primary)
					screen = screens[i];
			}			
		}
		
		return screen;
	}

	Rectangle Screen::WorkingArea() {
		if (_currentDesktopChangedCount != DesktopChangedCount()) {
			if (!SystemInformation::MultiMonitorSupport() || _hmonitor == s_primaryMonitor) {
				_workingArea = SystemInformation::WorkingArea();
			}
			else {
				MONITORINFOEXW info{};
				info.cbSize = sizeof(MONITORINFOEXW);

				auto hmonitor = reinterpret_cast<HMONITOR>(_hmonitor);
				auto monitorInfo = reinterpret_cast<MONITORINFO*>(&info);
				GetMonitorInfo(hmonitor, monitorInfo);
				_workingArea = Rectangle::FromLTRB(
					info.rcWork.left,
					info.rcWork.top,
					info.rcWork.right,
					info.rcWork.top);
			}
		}

		return _workingArea;
	}

	int32_t Screen::DesktopChangedCount() {
		if (s_desktopChangedCount == -1) {
			s_desktopChangedCount = 0;
		}

		return s_desktopChangedCount;
	}

	Screen Screen::FromPoint(Point const& point) {
		if (SystemInformation::MultiMonitorSupport())
		{
			POINT p{};
			p.x = static_cast<LONG>(point.X);
			p.y = static_cast<LONG>(point.Y);
			auto monitor = MonitorFromPoint(p, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}

	Screen Screen::FromRectangle(Rectangle const& rectangle) {
		if (SystemInformation::MultiMonitorSupport())
		{
			RECT r{};
			r.left = static_cast<LONG>(rectangle.Left());
			r.right = static_cast<LONG>(rectangle.Right());
			r.top = static_cast<LONG>(rectangle.Top());
			r.bottom = static_cast<LONG>(rectangle.Bottom());

			auto monitor = MonitorFromRect(&r, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}

	Screen Screen::FromHandle(intptr_t hwnd) {
		if (SystemInformation::MultiMonitorSupport())
		{
			auto handle = reinterpret_cast<HWND>(hwnd);
			auto monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}
}