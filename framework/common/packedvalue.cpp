#include "xna/common/packedvalue.hpp"

namespace xna {
	Uint PackUtils::PackUnsigned(float bitmask, float value) {
		return static_cast<Uint>(ClampAndRound(value, 0.0f, bitmask));
	}

	Uint PackUtils::PackSigned(Uint bitmask, float value) {
		const auto max = static_cast<float>(bitmask >> 1);
		const auto min = -max - 1.0F;

		return static_cast<Uint>(ClampAndRound(value, min, max)) & bitmask;
	}

	Uint PackUtils::PackUNorm(float bitmask, float value) {
		value *= bitmask;
		return static_cast<Uint>(ClampAndRound(value, 0.0f, bitmask));
	}

	Uint PackUtils::PackSNorm(Uint bitmask, float value) {
		const auto max = static_cast<float>(bitmask >> 1);
		value *= max;
		return static_cast<Uint>(ClampAndRound(value, -max, max)) & bitmask;
	}

	double PackUtils::ClampAndRound(float value, float min, float max) {
		if (isnan(value))
			return 0.0;

		if (isinf(value))
			return value < 0 ? static_cast<double>(min) : static_cast<double>(max);

		if (value < min)
			return static_cast<double>(min);

		return value > max ? static_cast<double>(max) : std::round(static_cast<double>(value));
	}
}