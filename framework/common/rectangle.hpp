#ifndef XNA_COMMON_RECTANGLE_HPP
#define XNA_COMMON_RECTANGLE_HPP

#include "../types.hpp"

namespace xna {
	struct Rectangle {
		Int Height{ 0 };
		Int Width{ 0 };
		Int X{ 0 };
		Int Y{ 0 };

		constexpr Rectangle() = default;

		constexpr Rectangle(const Int& Height, const Int& Width, const Int& X, const Int& Y)
			: Height(Height), Width(Width), X(X), Y(Y) {}
	};
}

#endif