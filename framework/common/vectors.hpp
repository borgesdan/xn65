#ifndef XNA_COMMON_VECTORS_HPP
#define XNA_COMMON_VECTORS_HPP

#include <cmath>
#include "../default.hpp"

namespace xna {
	struct Vector2 {
		float X{ 0 };
		float Y{ 0 };

		constexpr Vector2() = default;
		constexpr Vector2(float value) : X(value), Y(value) {}

		constexpr Vector2(float X, float Y)
			: X(X), Y(Y) {}

		constexpr bool operator==(const Vector2& other) const {
			return X == other.X && Y == other.Y;
		}

		static constexpr Vector2 Zero() { return {}; }
		static constexpr Vector2 One() { return { 1 }; }
		static constexpr Vector2 UnitX() { return { 1, 0 }; }
		static constexpr Vector2 UnitY() { return { 0, 1 }; }

		inline float Length() const {
			return sqrt(LengthSquared());
		}

		constexpr float LengthSquared() const {
			return (X * X + Y * Y);
		}

		static inline float Distance(Vector2 const& value1, Vector2 const& value2) {
			return sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector2 const& value1, Vector2 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			return x * x + y * y;
		}

		static constexpr float Dot(Vector2 const& value1, Vector2 const& value2) {
			return value1.X * value2.X + value1.Y * value2.Y;
		}

		inline void Normalize() {
			const auto normal = 1.0f / Length();
			X *= normal;
			Y *= normal;
		}

		static inline Vector2 Normalize(Vector2 const& value) {
			const auto normal = 1.0F / value.Length();
			return { value.X * normal, value.Y * normal };
		}

		static constexpr Vector2 Reflect(Vector2 const& vector, Vector2 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y;
			return { vector.X - 2.0f * num * normal.X , vector.Y - 2.0f * num * normal.Y };
		}

		static constexpr Vector2 Min(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y
			};
		}

		static constexpr Vector2 Max(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y
			};
		}

		static constexpr Vector2 Clamp(Vector2 const& value1, Vector2 const& min, Vector2 const& max) {
			const auto _maxx = value1.X > max.X ? max.X : value1.X;
			const auto _x = _maxx < min.X ? min.X : _maxx;
			const auto _maxy = value1.Y > max.Y ? max.Y : value1.Y;
			const auto _y = _maxy < min.Y ? min.Y : _maxy;

			return { _x, _y };
		}

		static constexpr Vector2 Lerp(Vector2 const& value1, Vector2 const& value2, float amount) {
			return{
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		static constexpr Vector2 Barycentric(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, float amount1, float amount2) {
			return {
				value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X),
				value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y)
			};
		}

		static constexpr Vector2 SmoothStep(Vector2 const& value1, Vector2 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = amount * amount * (3.0F - 2.0F * amount);

			return {
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		static constexpr Vector2 CatmullRom(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, Vector2 const& value4, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			Vector2 vector2;
			vector2.X = 0.5F * (2.0F * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0F * value2.X + 4.0F * value3.X - value4.X) * am2 + (-value1.X + 3.0F * value2.X - 3.0F * value3.X + value4.X) * am3);
			vector2.Y = 0.5F * (2.0F * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0F * value2.Y + 4.0F * value3.Y - value4.Y) * am2 + (-value1.Y + 3.0F * value2.Y - 3.0F * value3.Y + value4.Y) * am3);
			return vector2;
		}

		static constexpr Vector2 Hermite(Vector2 const& value1, Vector2 const& tangent1, Vector2 const& value2, Vector2 const& tangent2, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			const auto num3 = (2.0F * am3 - 3.0F * am2 + 1.0F);
			const auto num4 = (-2.0F * am3 + 3.0F * am2);
			const auto num5 = am3 - 2.0F * am2 + amount;
			const auto am4 = am3 - am2;

			return{
				value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * am4,
				value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * am4
			};
		}

		static constexpr Vector2 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector2 TransformNormal(Vector2 const& normal, Matrix const& matrix);
		static constexpr Vector2 Transform(Vector2 const& value, Quaternion const& rotation);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);

		static constexpr Vector2 Negate(Vector2 const& value) {
			return { -value.X, -value.Y };
		}

		static constexpr Vector2 Add(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X + value2.X, value1.Y + value2.Y };
		}

		static constexpr Vector2 Subtract(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X - value2.X, value1.Y - value2.Y };
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X * value2.X, value1.Y * value2.Y };
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, float scaleFactor) {
			return { value1.X * scaleFactor, value1.Y * scaleFactor };
		}

		static constexpr Vector2 Divide(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X / value2.X, value1.Y / value2.Y };
		}

		static constexpr Vector2 Divide(Vector2 const& value1, float divider) {
			return { value1.X / divider, value1.Y / divider };
		}

		constexpr Vector2 operator-() const {
			return Negate(*this);
		}

		friend constexpr Vector2 operator+(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Add(value1, value2);
		}

		friend constexpr Vector2 operator-(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Subtract(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Multiply(value1, value2);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Divide(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, float factor) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, float divider) {
			return Vector2::Divide(value1, divider);
		}

		friend constexpr Vector2 operator*(float factor, Vector2 const& value1) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(float divider, Vector2 const& value1) {
			return Vector2::Divide(value1, divider);
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

		static Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) { return {}; }
		static float Dot(Vector3 const& vector1, Vector3 const& vector2) { return 0; }
		static Vector3 Forward() { return {}; }
		static Vector3 Right() { return {}; }
		float LengthSquared() const { return 0; };
		static Vector3 Normalize(Vector3 const& value) { return {}; }
		static Vector3 Multiply(Vector3 value1, Vector3 value2) { return {}; }
		static Vector3 Subtract(Vector3 value1, Vector3 value2) { return {}; }
		static Vector3 Multiply(Vector3 value1, float value) { return {}; }
		void Normalize() {}

		constexpr Vector3 operator-() const {
			return Vector3();
		}

		friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) {
			return Vector3();
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