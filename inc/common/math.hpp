#ifndef XNA_COMMON_MATH_HPP
#define XNA_COMMON_MATH_HPP

#include <limits>
#include <cmath>

namespace xna {
	struct MathHelper {
		static constexpr double E = 2.7182818284590451;
		static constexpr double PI = 3.1415926535897931;
		static constexpr double TAU = 6.2831853071795862;
		static constexpr double EPSILON = 4.94065645841247E-324;

		static constexpr float ToRadians(float degrees) { return degrees * (static_cast<float>(PI) / 180.0f); }
		static constexpr float ToDegrees(float radians) { return radians * 57.2957764F; }
		static float Distance(float value1, float value2) { return std::abs(value1 - value2); }
		static constexpr float Min(float value1, float value2) { return (std::min)(value1, value2); }
		static constexpr float Max(float value1, float value2) { return (std::max)(value1, value2); }

		static constexpr float Clamp(float value, float min, float max) {
			value = value > max ? max : value;
			value = value < min ? min : value;
			return value;
		}

		static constexpr float Lerp(float value1, float value2, float amount) {
			return value1 + (value2 - value1) * amount;
		}

		static constexpr float Barycentric(float value1, float value2, float value3, float amount1, float amount2) {
			return value1 + amount1 * (value2 - value1) + amount2 * (value3 - value1);
		}

		static constexpr float SmoothStep(float value1, float value2, float amount) {
			const auto num = Clamp(amount, 0.0f, 1.0f);
			return Lerp(value1, value2, (num * num * (3.0F - 2.0F * num)));
		}

		static constexpr float CatmullRom(float value1, float value2, float value3, float value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			return (0.5F * (2.0F * value2 + (-value1 + value3) * amount + (2.0F * value1 - 5.0F * value2 + 4.0F * value3 - value4) * num1 + (-value1 + 3.0F * value2 - 3.0F * value3 + value4) * num2));
		}

		static constexpr float Hermite(float value1, float tangent1, float value2, float tangent2, float amount) {
			const auto num1 = amount;
			const auto num2 = num1 * num1;
			const auto num3 = num1 * num2;
			const auto num4 = (2.0F * num3 - 3.0F * num2 + 1.0F);
			const auto num5 = (-2.0F * num3 + 3.0F * num2);
			const auto num6 = num3 - 2.0f * num2 + num1;
			const auto num7 = num3 - num2;
			return value1 * num4 + value2 * num5 + tangent1 * num6 + tangent2 * num7;
		}
	};
}

#endif