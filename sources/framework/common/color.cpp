#include "xna/common/color.hpp"

namespace xna {
	Color::Color(float r, float g, float b, float a) :
		_packedValue(PackHelper(r, g, b, a)) {
	}

	Color::Color(Vector3 const& vector) :
		_packedValue(PackHelper(vector.X, vector.Y, vector.Z, 1.0F)) {
	}
	
	Color::Color(Vector4 const& vector) :
		_packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
	}

	void Color::PackFromVector4(Vector4 const& vector) {
		_packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
	}	

	Color Color::FromNonPremultiplied(Vector4 const& vector) {
		Color color;
		color._packedValue = PackHelper(vector.X * vector.W, vector.Y * vector.W, vector.Z * vector.W, vector.W);
		return color;
	}

	Color Color::FromNonPremultiplied(int32_t r, int32_t g, int32_t b, int32_t a) {
		r = ClampToByte32(r * a / ByteMaxValue);
		g = ClampToByte32(g * a / ByteMaxValue);
		b = ClampToByte32(b * a / ByteMaxValue);
		a = ClampToByte32(a);
		g <<= 8;
		b <<= 16;
		a <<= 24;

		Color color;
		color._packedValue = static_cast<uint32_t>(r | g | b | a);
		return color;
	}

	Color Color::Lerp(Color const& value1, Color const& value2, float amount) {
		const int32_t r1 = value1.R();
		const int32_t g1 = value1.G();
		const int32_t b1 = value1.B();
		const int32_t a1 = value1.A();
		const int32_t r2 = value2.R();
		const int32_t g2 = value2.G();
		const int32_t b2 = value2.B();
		const int32_t a2 = value2.A();

		const auto bitmask = static_cast<int32_t>(PackUtils::PackUNorm(65536.0f, amount));

		const int32_t r = r1 + ((r2 - r1) * bitmask >> 16);
		const int32_t g = g1 + ((g2 - g1) * bitmask >> 16);
		const int32_t b = b1 + ((b2 - b1) * bitmask >> 16);
		const int32_t a = a1 + ((a2 - a1) * bitmask >> 16);

		Color color;
		color._packedValue = static_cast<uint32_t>(r | g << 8 | b << 16 | a << 24);
		return color;
	}

	uint32_t Color::PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW) {
		const auto byteMax = static_cast<float>(ByteMaxValue);
		const auto x = PackUtils::PackUNorm(byteMax, vectorX);
		const auto y = PackUtils::PackUNorm(byteMax, vectorY) << 8;
		const auto z = PackUtils::PackUNorm(byteMax, vectorZ);
		const auto w = PackUtils::PackUNorm(byteMax, vectorW) << 24;

		return x | y | z | w;
	}
}