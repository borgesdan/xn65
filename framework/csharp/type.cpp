#include "csharp/type.hpp"

namespace xna {
	size_t Type::GetHashCode() const
	{
		size_t seed = 0;
		XnaHHashCombine(seed, fullName);
		XnaHHashCombine(seed, isClass);
		XnaHHashCombine(seed, isEnum);
		XnaHHashCombine(seed, isValueType);
		XnaHHashCombine(seed, isPrimitive);

		return seed;
	}	
}