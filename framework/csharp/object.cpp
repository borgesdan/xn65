#include "object.hpp"
#include "type.hpp"

namespace xna {
	sptr<Type> Object::GetType() const
	{
		auto type = New<Type>();
		type->FullName = "xna::Object";
		type->Namespace = "xna";
		type->IsClass = true;
		return type;
	}

	size_t Object::GetHashCode() const
	{
		size_t seed = 0;
		XnaHHashCombine(seed, this);

		return seed;
	}
}