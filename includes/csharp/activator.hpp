#ifndef CSHARP_ACTIVATOR_HPP
#define CSHARP_ACTIVATOR_HPP

#include <any>
#include "type.hpp"

namespace csharp {
	class Activator {
		static std::any CreateInstance(Type const& type);
	};
}

#endif