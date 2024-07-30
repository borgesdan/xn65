#include "xna/xna-dx.hpp"

namespace xna {
    
    //See ref
    //https://learn.microsoft.com/pt-br/windows/win32/api/winuser/nf-winuser-getmonitorinfoa
    //https://learn.microsoft.com/pt-br/windows/win32/api/winuser/ns-winuser-monitorinfoexa
    //https://stackoverflow.com/questions/7767036/how-do-i-get-the-number-of-displays-in-windows

    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
    {
        auto screens = (std::vector<uptr<Screen>>*)dwData;

        MONITORINFOEX monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(hMonitor, &monitorInfo);
        
        const auto hmonitor = reinterpret_cast<intptr_t>(hMonitor);

        const auto primary = screens->size() == 0;

        Rectangle bounds;
        bounds.X = monitorInfo.rcMonitor.left;
        bounds.Y = monitorInfo.rcMonitor.top;
        bounds.Width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        bounds.Height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

        Rectangle workingArea;
        workingArea.X = monitorInfo.rcWork.left;
        workingArea.Y = monitorInfo.rcWork.top;
        workingArea.Width = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
        workingArea.Height = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;

        const auto deviceName = String(monitorInfo.szDevice);

        auto screen = unew<Screen>(
            hmonitor,
            primary,
            bounds,
            workingArea,
            deviceName
        );       

        screens->push_back(std::move(screen));

        return TRUE;
    }   


	std::vector<uptr<Screen>> Screen::AllScreens() {
        std::vector<uptr<Screen>> screens;
        if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&screens))
            return screens;

        return std::vector<uptr<Screen>>();
	}
}