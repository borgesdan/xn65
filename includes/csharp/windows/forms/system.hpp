#ifndef CSHARP_WINDOWS_FORMS_SYSTEM_HPP
#define CSHARP_WINDOWS_FORMS_SYSTEM_HPP

#include "../../drawing/primitives.hpp"

namespace csharp {
	struct SystemInformation {
		static bool MultiMonitorSupport();
		static Rectangle VirtualScreen();
		static Size PrimaryMonitorSize();
		static Rectangle WorkingArea();

	private:
		static Size GetSize(int systemMetricsIndexX, int systemMetricsIndexY);

	private:
		inline static bool s_checkMultiMonitorSupport{ false };
		inline static bool s_multiMonitorSupport{ false };
	};
}

#endif