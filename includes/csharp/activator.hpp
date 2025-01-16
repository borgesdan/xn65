#ifndef CSHARP_ACTIVATOR_HPP
#define CSHARP_ACTIVATOR_HPP

#include <any>
#include <cstdint>
#include <string>
#include "type.hpp"
#include <optional>
#include <utility>
#include <vector>
#include <functional>
#include <memory>
#include "csharp/drawing/primitives.hpp"
#include "exception.hpp"

namespace csharp {
	class Activator {
	public:
		template <typename T, typename... Arguments>
		static std::any CreateInstance(Type_T<T> const& type, Arguments &&... args) {			
			using value_type = Type_T<T>::value_type;

			std::any obj = std::any(std::in_place_type<value_type>, std::forward<Arguments>(args)...);

			return obj;
		}	

		template <typename T, typename... Arguments>
		static std::any CreateInstance(Type const& type, Arguments &&... args) {
			auto type_t = reinterpret_cast<const Type_T<T>*>(&type);

			if (!type_t)
				throw csharp::InvalidOperationException();

			std::any obj = std::any(std::in_place_type<T>, std::forward<Arguments>(args)...);

			return obj;
		}
	};
}

#endif