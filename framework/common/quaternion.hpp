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

	constexpr Vector3 Vector3::Transform(Vector3 const& value, Quaternion const& rotation)
	{
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0F - num7 - num10));
		Vector3 vector3;
		vector3.X = num13;
		vector3.Y = num14;
		vector3.Z = num15;
		return vector3;
	}
}

#endif