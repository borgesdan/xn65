#ifndef XNA_FRAMEWORK_POINT_HPP
#define XNA_FRAMEWORK_POINT_HPP

#include <cstdint>
#include <optional>

namespace Xna {
	//Defines a point in 2D space.
	struct Point {
		int32_t X{ 0 };
		int32_t Y{ 0 };

		constexpr Point() = default;

		constexpr Point(const int32_t& X, const int32_t& Y)
			: X(X), Y(Y) {
		}

		constexpr bool operator==(const Point& other) const {
			return X == other.X && Y == other.Y;
		}

		constexpr operator std::optional<Point>() const {
			return std::make_optional<Point>(X, Y);
		}
	};
}

#endif