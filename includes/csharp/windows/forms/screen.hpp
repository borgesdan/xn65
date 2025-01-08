#ifndef CSHARP_WINDOWS_FORMS_SCREEN_HPP
#define CSHARP_WINDOWS_FORMS_SCREEN_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <memory>
#include "../../drawing/primitives.hpp"

namespace csharp {	
	class Screen {
	public:
		Screen(intptr_t monitor) : Screen(monitor, 0){}
		Screen(intptr_t monitor, intptr_t hdc);
		
		static std::vector<std::unique_ptr<Screen>> AllScreens();
		static std::unique_ptr<Screen> PrimaryScreen();

	private:
		static int s_primaryMonitor;

	private:
		intptr_t _hmonitor;
		Rectangle _bounds;
		Rectangle _workingArea;
		bool _primary;
		std::string _deviceName;
		int32_t _bitDepth;
	};
}

#endif