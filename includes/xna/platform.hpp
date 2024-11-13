#ifndef XNA_PLATFORM_HPP
#define XNA_PLATFORM_HPP

#include <memory>

namespace xna {
	template <typename T> struct PlatformImplementation {
		virtual ~PlatformImplementation() {}
		
		std::unique_ptr<T> Implementation;
	};
}

#endif