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
			auto v = value;
			v.Normalize();
			return v;
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
		static bool Transform(std::vector<Vector2> sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		static bool TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static bool TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength);
		static bool Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray);
		static bool Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);

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

		constexpr Vector3(float value)
			: X(value), Y(value), Z(value) { }

		constexpr Vector3(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z) { }

		constexpr Vector3(Vector2 const& value, float z)
			: X(value.X), Y(value.Y), Z(z) { }

		constexpr bool operator==(const Vector3& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		static Vector3 Zero() { return {}; }
		static Vector3 One() { return { 1 }; }
		static Vector3 UnitX() { return { 1,0,0 }; }
		static Vector3 UnitY() { return { 0,1,0 }; }
		static Vector3 UnitZ() { return { 0,0,1 }; }
		static Vector3 Up() { return UnitY(); }
		static Vector3 Down() { return -UnitY(); }
		static Vector3 Right() { return UnitX(); }
		static Vector3 Left() { return -UnitX(); }
		static Vector3 Forward() { return -UnitZ(); }
		static Vector3 Backward() { return UnitZ(); }

		inline float Length() const { return sqrt(LengthSquared());	}
		constexpr float LengthSquared() const {	return (X * X + Y * Y + Z * Z);	}

		inline static float Distance(Vector3 const& value1, Vector3 const& value2) {
			return std::sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector3 const& value1, Vector3 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			const auto z = value1.Z - value2.Z;
			return x * x + y * y + z * z;
		}

		static float Dot(Vector3 const& vector1, Vector3 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
		}

		inline void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
		}
		
		static Vector3 Normalize(Vector3 const& value) {
			auto v = value;
			v.Normalize();
			return v;
		}

		static constexpr Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) {
			Vector3 vector3;
			vector3.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;
			vector3.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;
			vector3.Z = vector1.X * vector2.Y - vector1.Y * vector2.X;
			return vector3;
		}
		
		static constexpr Vector3 Reflect(Vector3 const& vector, Vector3 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;
			Vector3 vector3;
			vector3.X = vector.X - 2.0f * num * normal.X;
			vector3.Y = vector.Y - 2.0f * num * normal.Y;
			vector3.Z = vector.Z - 2.0f * num * normal.Z;
			return vector3;
		}

		static constexpr Vector3 Min(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X < value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		static constexpr Vector3 Max(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X >  value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y >  value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z >  value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		static constexpr Vector3 Clamp(Vector3 const& value1, Vector3 const& min, Vector3 const& max) {
			const auto x = value1.X > max.X ? max.X : value1.X;
			const auto _x = x < min.X ? min.X : x;
			const auto y = value1.Y >  max.Y ? max.Y : value1.Y;
			const auto _y = y < min.Y ? min.Y : y;
			const auto z = value1.Z >  max.Z ? max.Z : value1.Z;
			const auto _z = z < min.Z ? min.Z : z;

			Vector3 vector3;
			vector3.X = _x;
			vector3.Y = _y;
			vector3.Z = _z;
			return vector3;
		}

		static constexpr Vector3 Lerp(Vector3 const& value1, Vector3 const& value2, float amount) {
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		static constexpr Vector3 Barycentric(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3,	float amount1, float amount2) {
			Vector3 vector3;
			vector3.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector3.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector3.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			return vector3;
		}

		static constexpr Vector3 SmoothStep(Vector3 const& value1, Vector3 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0f ? 0.0f : amount);
			amount = (amount * amount * (3.0f - 2.0f * amount));
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		static constexpr Vector3 CatmullRom(Vector3 const& value1, Vector3 const& value2,	Vector3 const& value3, Vector3 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector3 vector3;
			vector3.X = 0.5f * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0f * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector3.Y = 0.5f * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0f * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector3.Z = 0.5f * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0f * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			return vector3;
		}

		static Vector3 Hermite(Vector3 const& value1, Vector3 const& tangent1, Vector3 const& value2, Vector3 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = 2.0F * num2 - 3.0F * num1 + 1.0F;
			const auto num4 = -2.0F * num2 + 3.0F * num1;
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector3 vector3;
			vector3.X = value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6;
			vector3.Y = value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6;
			vector3.Z = value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6;
			return vector3;
		}

		static constexpr Vector3 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector3 TransformNormal(Vector3 const& normal, Matrix const& matrix);
		static constexpr Vector3 Transform(Vector3 const& value, Quaternion const& rotation);
		static bool Transform(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static bool Transform(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destionationArrayLength);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation,Vector3* destinationArray, size_t destinationLength);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray);
		static bool TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);

		static constexpr Vector3 Negate(Vector3 const& value)
		{
			Vector3 vector3;
			vector3.X = -value.X;
			vector3.Y = -value.Y;
			vector3.Z = -value.Z;
			return vector3;
		}

		static constexpr Vector3 Add(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X + value2.X;
			vector3.Y = value1.Y + value2.Y;
			vector3.Z = value1.Z + value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Subtract(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X - value2.X;
			vector3.Y = value1.Y - value2.Y;
			vector3.Z = value1.Z - value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Multiply(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X * value2.X;
			vector3.Y = value1.Y * value2.Y;
			vector3.Z = value1.Z * value2.Z;
			return vector3;
		}
		
		static constexpr Vector3 Multiply(Vector3 const& value1, float scaleFactor) {
			Vector3 vector3;
			vector3.X = value1.X * scaleFactor;
			vector3.Y = value1.Y * scaleFactor;
			vector3.Z = value1.Z * scaleFactor;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X / value2.X;
			vector3.Y = value1.Y / value2.Y;
			vector3.Z = value1.Z / value2.Z;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, float divider) {
			float num = 1.0f / divider;
			Vector3 vector3;
			vector3.X = value1.X * num;
			vector3.Y = value1.Y * num;
			vector3.Z = value1.Z * num;
			return vector3;
		}

		constexpr Vector3 operator-() const {
			return Vector3::Negate(*this);
		}

		friend constexpr Vector3 operator+(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Add(value1, value2);
		}

		friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Subtract(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Multiply(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value, float factor) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator*(float factor, Vector3 const& value) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator/(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Divide(value1, value2);
		}

		friend constexpr Vector3 operator/(Vector3 const& value, float divider) {
			return Vector3::Divide(value, divider);
		}

		friend constexpr Vector3 operator/(float divider, Vector3 const& value) {
			return Vector3::Divide(value, divider);
		}
	};

	struct Vector4 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };

		constexpr Vector4() = default;

		constexpr Vector4(float value):
			X(value), Y(value), Z(value), W(value){}

		constexpr Vector4(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) { }

		constexpr Vector4(Vector2 value, float Z, float W)
			: X(value.X), Y(value.Y), Z(Z), W(W) { }

		constexpr Vector4(Vector3 value, float W)
			: X(value.X), Y(value.Y), Z(value.Z), W(W) { }

		constexpr bool operator==(const Vector4& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		static Vector4 Zero() { return {}; }
		static Vector4 One() { return { 1 }; }
		static Vector4 UnitX() { return { 1,0,0,0 }; }
		static Vector4 UnitY() { return { 0,1,0,0 }; }
		static Vector4 UnitZ() { return { 0,0,1,0 }; }
		static Vector4 UnitW() { return { 0,0,0,1 }; }

		float Length() const {
			return sqrt(LengthSquared());
		}

		constexpr float LengthSquared() const {
			return (X * X + Y * Y + Z * Z + W * W);
		}

		inline static float Distance(Vector4 const& value1, Vector4 const& value2) {
			return sqrt(DistanceSquared(value1, value2));
		}

		static constexpr float DistanceSquared(Vector4 const& value1, Vector4 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4;
		}

		static constexpr float Dot(Vector4 const& vector1, Vector4 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z + vector1.W * vector2.W;
		}

		void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		inline static Vector4 Normalize(Vector4 const& vector) {
			auto v = vector;
			v.Normalize();
			return v;
		}

		static constexpr Vector4 Min(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X < value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W < value2.W ? value1.W : value2.W;
			return vector4;
		}

		static constexpr Vector4 Max(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X >  value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y >  value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z >  value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W > value2.W ? value1.W : value2.W;
			return vector4;
		}

		static Vector4 Clamp(Vector4 const& value1, Vector4 const& min, Vector4 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;
			const auto y = value1.Y;
			const auto num3 = y >  max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;
			const auto z = value1.Z;
			const auto num5 = z >  max.Z ? max.Z : z;
			const auto num6 = num5 < min.Z ? min.Z : num5;
			const auto w = value1.W;
			const auto num7 = w >  max.W ? max.W : w;
			const auto num8 = num7 < min.W ? min.W : num7;
			Vector4 vector4;
			vector4.X = num2;
			vector4.Y = num4;
			vector4.Z = num6;
			vector4.W = num8;
			return vector4;
		}

		static Vector4 Lerp(Vector4 const& value1, Vector4 const& value2, float amount) {
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static Vector4 Barycentric(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3,	float amount1,	float amount2) {
			Vector4 vector4;
			vector4.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector4.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector4.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			vector4.W = value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W);
			return vector4;
		}

		static Vector4 SmoothStep(Vector4 const& value1, Vector4 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = (amount * amount * (3.0F - 2.0F * amount));
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static Vector4 CatmullRom(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, Vector4 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector4 vector4;
			vector4.X = 0.5F * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector4.Y = 0.5F * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector4.Z = 0.5F * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0F * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			vector4.W = 0.5F * (2.0f * value2.W + (-value1.W + value3.W) * amount + (2.0F * value1.W - 5.0f * value2.W + 4.0f * value3.W - value4.W) * num1 + (-value1.W + 3.0f * value2.W - 3.0f * value3.W + value4.W) * num2);
			return vector4;
		}

		static Vector4 Hermite(Vector4 const& value1, Vector4 const& tangent1, Vector4 const& value2, Vector4 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = (2.0F * num2 - 3.0F * num1 + 1.0F);
			const auto num4 = (-2.0F * num2 + 3.0F * num1);
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector4 vector4;
			vector4.X = (value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6);
			vector4.Y = (value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6);
			vector4.Z = (value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6);
			vector4.W = (value1.W * num3 + value2.W * num4 + tangent1.W * num5 + tangent2.W * num6);
			return vector4;
		}

		static constexpr Vector4 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector4 const& vector, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector2 const& value, Quaternion const& rotation);
		static constexpr Vector4 Transform(Vector3 const& value, Quaternion const& rotation);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceLength, Matrix const& matrix, std::vector<Vector4>& destinationArray);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength);
		static bool Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray);
		static bool Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		static bool Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);

		static constexpr Vector4 Negate(Vector4 const& value) {
			Vector4 vector4;
			vector4.X = -value.X;
			vector4.Y = -value.Y;
			vector4.Z = -value.Z;
			vector4.W = -value.W;
			return vector4;
		}		

		static constexpr Vector4 Add(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X + value2.X;
			vector4.Y = value1.Y + value2.Y;
			vector4.Z = value1.Z + value2.Z;
			vector4.W = value1.W + value2.W;
			return vector4;
		}

		static constexpr Vector4 Subtract(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X - value2.X;
			vector4.Y = value1.Y - value2.Y;
			vector4.Z = value1.Z - value2.Z;
			vector4.W = value1.W - value2.W;
			return vector4;
		}		

		static constexpr Vector4 Multiply(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X * value2.X;
			vector4.Y = value1.Y * value2.Y;
			vector4.Z = value1.Z * value2.Z;
			vector4.W = value1.W * value2.W;
			return vector4;
		}
		

		static constexpr Vector4 Multiply(Vector4 const& value1, float scaleFactor)
		{
			Vector4 vector4;
			vector4.X = value1.X * scaleFactor;
			vector4.Y = value1.Y * scaleFactor;
			vector4.Z = value1.Z * scaleFactor;
			vector4.W = value1.W * scaleFactor;
			return vector4;
		}		

		static constexpr Vector4 Divide(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X / value2.X;
			vector4.Y = value1.Y / value2.Y;
			vector4.Z = value1.Z / value2.Z;
			vector4.W = value1.W / value2.W;
			return vector4;
		}		

		static constexpr Vector4 Divide(Vector4 const& value1, float divider)
		{
			float num = 1.0f / divider;
			Vector4 vector4;
			vector4.X = value1.X * num;
			vector4.Y = value1.Y * num;
			vector4.Z = value1.Z * num;
			vector4.W = value1.W * num;
			return vector4;
		}	

		constexpr Vector4 operator-() const {
			return Vector4::Negate(*this);
		}

		friend constexpr Vector4 operator+(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Add(value1, value2);
		}

		friend constexpr Vector4 operator-(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Subtract(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Multiply(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value, float factor) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator*(float factor, Vector4 const& value) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator/(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Divide(value1, value2);
		}

		friend constexpr Vector4 operator/(Vector4 const& value, float divider) {
			return Vector4::Divide(value, divider);
		}

		friend constexpr Vector4 operator/(float divider, Vector4 const& value) {
			return Vector4::Divide(value, divider);
		}
	};
}

#endif