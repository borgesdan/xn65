#include "csharp/windows/forms/screen.hpp"
#include "csharp/windows/forms/system.hpp"
#include <limits>

#ifdef WIN32
#include "Windows.h"

namespace csharp {
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

	std::unique_ptr<Screen> Screen::FromPoint(Point const& point) {
		if (SystemInformation::MultiMonitorSupport())
		{
			POINT p{};
			p.x = static_cast<LONG>(point.X);
			p.y = static_cast<LONG>(point.Y);
			auto monitor = MonitorFromPoint(p, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return std::make_unique<Screen>(i_monitor);
		}

		return std::make_unique<Screen>(s_primaryMonitor);
	}

	std::unique_ptr<Screen> Screen::FromRectangle(Rectangle const& rectangle) {
		if (SystemInformation::MultiMonitorSupport())
		{
			RECT r{};
			r.left = static_cast<LONG>(rectangle.Left());
			r.right = static_cast<LONG>(rectangle.Right());
			r.top = static_cast<LONG>(rectangle.Top());
			r.bottom = static_cast<LONG>(rectangle.Bottom());

			auto monitor = MonitorFromRect(&r, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return std::make_unique<Screen>(i_monitor);
		}

		return std::make_unique<Screen>(s_primaryMonitor);
	}

	std::unique_ptr<Screen> Screen::FromHandle(intptr_t hwnd) {
		if (SystemInformation::MultiMonitorSupport())
		{
			auto handle = reinterpret_cast<HWND>(hwnd);
			auto monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST);
			auto i_monitor = reinterpret_cast<intptr_t>(monitor);
			return std::make_unique<Screen>(i_monitor);
		}

		return std::make_unique<Screen>(s_primaryMonitor);
	}	
}

#endif