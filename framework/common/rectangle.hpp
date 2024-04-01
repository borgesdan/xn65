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

		constexpr Rectangle(const Int& X, const Int& Y, const Int& Width, const Int& Height):
			X(X), Y(Y), Width(Width), Height(Height) {}
	};
}

#endif