#ifndef XNA_COMMON_COLOR_HPP
#define XNA_COMMON_COLOR_HPP

#include "../default.hpp"

namespace xna {
	struct Color {
		constexpr Color() = default;

		Uint packedValue{0};

		constexpr bool operator==(const Color& other) const {
			return packedValue == other.packedValue;
		}
	};
}

#endif