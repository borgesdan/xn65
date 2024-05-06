#ifndef XNA_PLATFORM_INIT_HPP
#define XNA_PLATFORM_INIT_HPP

#include "../default.hpp"

namespace xna {
	struct InitPlatform {
		static void Init() {
			InitRegisteredTypes();
			InitActivadors();
		}

		static void InitRegisteredTypes();
		static void InitActivadors();
	};	
}

#endif