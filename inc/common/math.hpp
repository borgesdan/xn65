#ifndef XNA_COMMON_MATH_HPP
#define XNA_COMMON_MATH_HPP

#include <limits>
#include <cmath>

namespace xna {
	struct MathHelper {
		static constexpr double E = 2.7182818284590452354;
		static constexpr double PI = 3.14159265358979323846;
		static constexpr double TAU = PI * 2;		
		static constexpr double EPSILON = std::numeric_limits<double>::epsilon();

		static constexpr float ToRadians(float degrees) { return degrees * (static_cast<float>(PI) / 180.0f); }
		static constexpr float ToDegrees(float radians) { return radians * 57.2957764F; }
		static float Distance(float value1, float value2) { return std::abs(value1 - value2); }
		static float Min(float value1, float value2) { return (std::min)(value1, value2); }
		static float Max(float value1, float value2) { return (std::max)(value1, value2); }

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

		static float CatmullRom(float value1, float value2, float value3, float value4, float amount);
		static float Hermite(float value1, float tangent1, float value2, float tangent2, float amount);
	};
}

#endif