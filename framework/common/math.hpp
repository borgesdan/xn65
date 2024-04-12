#ifndef XNA_COMMON_MATH_HPP
#define XNA_COMMON_MATH_HPP

#include <limits>

namespace xna {
	struct MathHelper {
		static constexpr double E = 2.7182818284590451;
		static constexpr double PI = 3.1415926535897931;
		static constexpr double TAU = 6.2831853071795862;		
		static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
	};

	using Math = MathHelper;
}

#endif