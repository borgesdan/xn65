#ifndef XNA_COMMON_VECTORS_HPP
#define XNA_COMMON_VECTORS_HPP

namespace xna {
	struct Vector2 {
		float X{ 0 };
		float Y{ 0 };

		constexpr Vector2() = default;

		constexpr Vector2(float X, float Y)
			: X(X), Y(Y) {}

		constexpr bool operator==(const Vector2& other) const {
			return X == other.X && Y == other.Y;
		}
	};

	struct Vector3 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };

		constexpr Vector3() = default;

		constexpr Vector3(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z) { }

		constexpr bool operator==(const Vector3& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}
	};

	struct Vector4 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };

		constexpr Vector4() = default;

		constexpr Vector4(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) { }

		constexpr bool operator==(const Vector4& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}
	};
}

#endif