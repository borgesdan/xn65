#ifndef XNA_GRAPHICS_SHARED_HPP
#define XNA_GRAPHICS_SHARED_HPP

namespace xna {
	enum class ComparisonFunction {
		Never,
		Less,
		Equal,
		LessEquals,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	using CompareFunction = ComparisonFunction;
}

#endif