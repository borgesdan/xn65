#ifndef CXNA_COMMON_PACKEDVECTOR_HPP
#define CXNA_COMMON_PACKEDVECTOR_HPP

#include "../default.hpp"
#include "numerics.hpp"
#include <cmath>

namespace xna {
	class IPackedVector {
		virtual Vector4 ToVector4() const = 0;
		virtual void PackFromVector4(Vector4 const& vector) = 0;
	};

	template <typename T>
	class IPackedVectorT {
		virtual T PackedValue() const = 0;
		virtual void PackedValue(T const& value) = 0;
	};

	struct PackUtils {
		static Uint PackUnsigned(float bitmask, float value) {
			return static_cast<Uint>(ClampAndRound(value, 0.0f, bitmask));
		}

		static Uint PackSigned(Uint bitmask, float value) {
			const auto max = static_cast<float>(bitmask >> 1);
			const auto min = -max - 1.0F;

			return static_cast<Uint>(ClampAndRound(value, min, max)) & bitmask;
		}

		static Uint PackUNorm(float bitmask, float value) {
			value *= bitmask;
			return static_cast<Uint>(ClampAndRound(value, 0.0f, bitmask));
		}

		static constexpr float UnpackUNorm(Uint bitmask, Uint value) {
			value &= bitmask;
			return static_cast<float>(value) / static_cast<float>(bitmask);
		}

		static Uint PackSNorm(Uint bitmask, float value) {
			const auto max = static_cast<float>(bitmask >> 1);
			value *= max;
			return static_cast<Uint>(ClampAndRound(value, -max, max)) & bitmask;
		}

		static constexpr float UnpackSNorm(Uint bitmask, Uint value) {
			const auto num1 = (bitmask + 1U) >> 1;

			const auto ivalue = static_cast<Int>(value);
			const auto inum1 = static_cast<Int>(num1);
			const auto ibitmask = static_cast<Int>(bitmask);

			if ((ivalue & inum1) != 0) {
				if ((ivalue & ibitmask) == inum1)
					return -1.0f;

				value |= ~bitmask;
			}
			else
				value &= bitmask;

			const auto num2 = static_cast<float>(bitmask >> 1);

			return static_cast<Int>(value) / num2;
		}

		static double ClampAndRound(float value, float min, float max) {
			if (isnan(value))
				return 0.0;

			if (isinf(value))
				return value < 0 ? static_cast<double>(min) : static_cast<double>(max);

			if (value < min)
				return static_cast<double>(min);

			return value > max ? static_cast<double>(max) : std::round(static_cast<double>(value));
		}
	};
}

#endif