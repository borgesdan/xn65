#ifndef XNA_COMMON_POINT_HPP
#define XNA_COMMON_POINT_HPP

#include "../types.hpp"

namespace xna {
	struct Point {
		Int X{0};
		Int Y{ 0 };

		constexpr Point() = default;

		constexpr Point(const Int& X, const Int& Y)
			: X(X), Y(Y) {}

		constexpr bool operator==(const Point& other) const	{
			return X == other.X && Y == other.Y;
		}
	};
}

#endif