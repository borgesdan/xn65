#include "xna/csharp/type.hpp"

namespace xna {
	size_t Type::GetHashCode() const
	{
		size_t seed = 0;
		XnaHelper::HashCombine(seed, fullName);
		XnaHelper::HashCombine(seed, isClass);
		XnaHelper::HashCombine(seed, isEnum);
		XnaHelper::HashCombine(seed, isValueType);
		XnaHelper::HashCombine(seed, isPrimitive);

		return seed;
	}	
}