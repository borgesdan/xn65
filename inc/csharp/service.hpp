#ifndef XNA_CSHARP_SERVICE_HPP
#define XNA_CSHARP_SERVICE_HPP

#include "../default.hpp"
#include "type.hpp"
#include <any>

namespace xna {
	class IServiceProvider {
	public:
		virtual std::any GetService(Type& serviceType) = 0;
	};
}

#endif