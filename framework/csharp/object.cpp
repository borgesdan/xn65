#include "csharp/object.hpp"
#include "csharp/type.hpp"

namespace xna {
	size_t Object::GetHashCode() const
	{
		size_t seed = 0;
		XnaHelper::HashCombine(seed, this);

		return seed;
	}
}