#ifndef XNA_CSHARP_ACTIVATOR_HPP
#define XNA_CSHARP_ACTIVATOR_HPP

#include "../default.hpp"
#include "type.hpp"
#include <any>
#include <functional>
#include <map>
#include <utility>

namespace xna {
	class Activator {
	public:
		template <class _Ty, class... _Types>
		uptr<_Ty> CreateInstance(_Types&&... _Args) {
			return unew(_Args);
		}

	private:
		Activator() = default;
		Activator(Activator&) = default;
		Activator(Activator&&) = default;
	};
}

#endif