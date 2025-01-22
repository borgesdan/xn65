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
#include "exception.hpp"

namespace csharp {
	class Activator {
	public:
		template <typename T, typename... Arguments>
		static std::any CreateInstance(Type const& type, Arguments &&... args) {
			auto hashCode = type.GetHashCode();
			auto typeHash = csharp::typeof<T>().GetHashCode();

			if (hashCode != typeHash)
				throw csharp::InvalidOperationException();

			std::any obj = std::any(std::in_place_type<T>, std::forward<Arguments>(args)...);

			return obj;
		}		
	};
}

#endif