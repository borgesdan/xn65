#ifndef XNA_COMMON_QUATERNION_HPP
#define XNA_COMMON_QUATERNION_HPP

#include "vectors.hpp"

namespace xna {
	struct Quaternion {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };

		constexpr Quaternion() = default;

		constexpr Quaternion(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) { }

		constexpr bool operator==(const Quaternion& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
	};

	constexpr Vector2 Vector2::Transform(Vector2 const& value, Quaternion const& rotation) {
		const auto rx = rotation.X + rotation.X;
		const auto ry = rotation.Y + rotation.Y;
		const auto rz = rotation.Z + rotation.Z;
		const auto rwz = rotation.W * rz;
		const auto rxx = rotation.X * rx;
		const auto rxy = rotation.X * ry;
		const auto ryy = rotation.Y * ry;
		const auto rzz = rotation.Z * rz;
		const auto x = value.X * (1.0F - ryy - rzz) + value.Y * (rxy - rwz);
		const auto y = value.X * (rxy + rwz) + value.Y * (1.0F - rxx - rzz);
		return{ x,y };
	}


}

#endif