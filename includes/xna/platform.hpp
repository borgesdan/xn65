#ifndef XNA_PLATFORM_HPP
#define XNA_PLATFORM_HPP

#include <memory>

namespace xna {
	struct ImplementationBase {
		struct PlatformImplementation;
		std::unique_ptr<PlatformImplementation> Implementation;
	};
}

#endif