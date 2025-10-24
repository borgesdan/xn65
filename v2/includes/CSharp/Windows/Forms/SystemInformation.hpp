#ifndef CSHARP_WINDOWS_FORMS_SYSTEMINFORMATION_HPP
#define CSHARP_WINDOWS_FORMS_SYSTEMINFORMATION_HPP

#include "../../Drawing/Rectangle.hpp"

namespace CSharp {
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