#include "csharp/windows/forms/system.hpp"
//TODO: REMOVER
#include "csharp/type.hpp"
#include "csharp/activator.hpp"

#ifdef WIN32
#include <Windows.h>

namespace csharp {
	bool SystemInformation::MultiMonitorSupport() {
		if (!s_checkMultiMonitorSupport) {
			s_multiMonitorSupport = GetSystemMetrics(SM_CMONITORS) != 0;
			s_checkMultiMonitorSupport = true;
		}

		//TODO: REMOVER
		auto t = typeof<RECT>();

		return s_multiMonitorSupport;
	}

	Rectangle SystemInformation::VirtualScreen() {
		if (MultiMonitorSupport()) {
			const auto x = GetSystemMetrics(SM_XVIRTUALSCREEN);
			const auto y = GetSystemMetrics(SM_YVIRTUALSCREEN);
			const auto w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
			const auto h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

			return Rectangle(x, y, w, h);
		}

		const auto size = PrimaryMonitorSize();
		return Rectangle(0, 0, size.Width, size.Height);
	}

	Size SystemInformation::PrimaryMonitorSize() {
		return GetSize(SM_CXSCREEN, SM_CYSCREEN);
	}

	Size SystemInformation::GetSize(int systemMetricsIndexX, int systemMetricsIndexY) {
		const auto x = GetSystemMetrics(systemMetricsIndexX);
		const auto y = GetSystemMetrics(systemMetricsIndexY);
		return Size(x, y);
	}

	Rectangle SystemInformation::WorkingArea() {
		RECT workingArea{};
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &workingArea, NULL);
		Rectangle rect = Rectangle::FromLTRB(
			workingArea.left,
			workingArea.top,
			workingArea.right,
			workingArea.bottom);

		return rect;
	}
}

#endif