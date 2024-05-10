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

		constexpr Quaternion(Vector3 vectorPart, float scalarPart)
			: X(vectorPart.X), Y(vectorPart.Y), Z(vectorPart.Z), W(scalarPart) { }

		constexpr bool operator==(const Quaternion& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		static constexpr Quaternion Identity() {
			return { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		float constexpr LengthSquared() const {
			return X * X + Y * Y + Z * Z + W * W;
		}

		inline float Length() const { return std::sqrt(LengthSquared()); }

		void Normalize() {
			const auto num = 1.0F / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		static Quaternion Normalize(Quaternion const& quaternion) {
			auto q = quaternion;
			q.Normalize();
			return q;
		}

		constexpr void Conjugate() {
			X = -X;
			Y = -Y;
			Z = -Z;
		}

		static constexpr Quaternion Conjugate(Quaternion const& value) {
			Quaternion quaternion;
			quaternion.X = -value.X;
			quaternion.Y = -value.Y;
			quaternion.Z = -value.Z;
			quaternion.W = value.W;
			return quaternion;
		}

		static constexpr Quaternion Inverse(Quaternion const& quaternion) {
			const auto num = 1.0f / quaternion.LengthSquared();
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X * num;
			quaternion1.Y = -quaternion.Y * num;
			quaternion1.Z = -quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		static Quaternion CreateFromAxisAngle(Vector3 const& axis, float angle);
		static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static Quaternion CreateFromRotationMatrix(Matrix const& matrix);

		static constexpr float Dot(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
		}

		static Quaternion Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount);
		static Quaternion Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount);
		
		static constexpr Quaternion Concatenate(Quaternion const& value1, Quaternion const& value2) {
			const auto x1 = value2.X;
			const auto y1 = value2.Y;
			const auto z1 = value2.Z;
			const auto w1 = value2.W;
			const auto x2 = value1.X;
			const auto y2 = value1.Y;
			const auto z2 = value1.Z;
			const auto w2 = value1.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Negate(Quaternion const& quaternion) {
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X;
			quaternion1.Y = -quaternion.Y;
			quaternion1.Z = -quaternion.Z;
			quaternion1.W = -quaternion.W;
			return quaternion1;
		}

		static constexpr Quaternion Add(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X + quaternion2.X;
			quaternion.Y = quaternion1.Y + quaternion2.Y;
			quaternion.Z = quaternion1.Z + quaternion2.Z;
			quaternion.W = quaternion1.W + quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Subtract(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X - quaternion2.X;
			quaternion.Y = quaternion1.Y - quaternion2.Y;
			quaternion.Z = quaternion1.Z - quaternion2.Z;
			quaternion.W = quaternion1.W - quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x1 = quaternion1.X;
			const auto y1 = quaternion1.Y;
			const auto z1 = quaternion1.Z;
			const auto w1 = quaternion1.W;
			const auto x2 = quaternion2.X;
			const auto y2 = quaternion2.Y;
			const auto z2 = quaternion2.Z;
			const auto w2 = quaternion2.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, float scaleFactor) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X * scaleFactor;
			quaternion.Y = quaternion1.Y * scaleFactor;
			quaternion.Z = quaternion1.Z * scaleFactor;
			quaternion.W = quaternion1.W * scaleFactor;
			return quaternion;
		}

		static constexpr Quaternion Divide(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x = quaternion1.X;
			const auto y = quaternion1.Y;
			const auto z = quaternion1.Z;
			const auto w = quaternion1.W;
			const auto num1 = 1.0f / (quaternion2.X * quaternion2.X + quaternion2.Y * quaternion2.Y + quaternion2.Z * quaternion2.Z + quaternion2.W * quaternion2.W);
			const auto num2 = -quaternion2.X * num1;
			const auto num3 = -quaternion2.Y * num1;
			const auto num4 = -quaternion2.Z * num1;
			const auto num5 = quaternion2.W * num1;
			const auto num6 = y * num4 - z * num3;
			const auto num7 = z * num2 - x * num4;
			const auto num8 = x * num3 - y * num2;
			const auto num9 = x * num2 + y * num3 + z * num4;
			Quaternion quaternion;
			quaternion.X = (x * num5 + num2 * w) + num6;
			quaternion.Y = (y * num5 + num3 * w) + num7;
			quaternion.Z = (z * num5 + num4 * w) + num8;
			quaternion.W = w * num5 - num9;
			return quaternion;
		}

		constexpr Quaternion operator-(Quaternion const& quaternion) const {
			return Quaternion::Negate(quaternion);
		}

		constexpr friend Quaternion operator+(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator-(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Multiply(q1, q2);
		}

		constexpr friend Quaternion operator/(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Divide(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, float scaleValue) {
			return Quaternion::Multiply(q1, scaleValue);
		}

		constexpr friend Quaternion operator*(float scaleValue, Quaternion const& q1) {
			return Quaternion::Multiply(q1, scaleValue);
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

	constexpr Vector4 Vector4::Transform(Vector2 const& value, Quaternion const& rotation)
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
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& value, Quaternion const& rotation) {
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
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}
}

#endif