#ifndef CSHARP_ACTIVATOR_HPP
#define CSHARP_ACTIVATOR_HPP

#include <any>
#include <cstdint>
#include "type.hpp"
#include "optional"

namespace csharp {
	class Activator {
	public:
		static std::any CreateInstance(Type const& type) {
			const auto typeHashCode = type.GetHashCode();
			
			if (typeHashCode == int32Hash)
				return int();

			return std::string();
		}

	private:
		static inline const size_t int8Hash = typeof<int8_t>()->GetHashCode();
		static inline const size_t int16Hash = typeof<int16_t>()->GetHashCode();
		static inline const size_t int32Hash = typeof<int32_t>()->GetHashCode();
		static inline const size_t int64Hash = typeof<int64_t>()->GetHashCode();
	};
}

#endif