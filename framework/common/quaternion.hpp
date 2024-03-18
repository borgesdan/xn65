#ifndef XNA_COMMON_QUATERNION_HPP
#define XNA_COMMON_QUATERNION_HPP

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
}

#endif