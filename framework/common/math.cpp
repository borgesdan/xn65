#include "common/math.hpp"

namespace xna {
	float MathHelper::CatmullRom(float value1, float value2, float value3, float value4, float amount) {
		const auto num1 = amount * amount;
		const auto num2 = amount * num1;
		return (0.5F * (2.0F * value2 + (-value1 + value3) * amount + (2.0F * value1 - 5.0F * value2 + 4.0F * value3 - value4) * num1 + (-value1 + 3.0F * value2 - 3.0F * value3 + value4) * num2));
	}

	float MathHelper::Hermite(float value1, float tangent1, float value2, float tangent2, float amount) {
		const auto num1 = amount;
		const auto num2 = num1 * num1;
		const auto num3 = num1 * num2;
		const auto num4 = (2.0F * num3 - 3.0F * num2 + 1.0F);
		const auto num5 = (-2.0F * num3 + 3.0F * num2);
		const auto num6 = num3 - 2.0f * num2 + num1;
		const auto num7 = num3 - num2;
		return value1 * num4 + value2 * num5 + tangent1 * num6 + tangent2 * num7;
	}
}