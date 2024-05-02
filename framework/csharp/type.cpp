#include "type.hpp"

namespace xna {
	sptr<Type> Type::GetType() const
	{
		auto type = New<Type>();
		type->FullName = "xna::Type";
		type->Namespace = "xna";
		type->IsClass = true;
		return type;
	}

	size_t Type::GetHashCode() const
	{
		size_t seed = 0;
		XnaHHashCombine(seed, Namespace);
		XnaHHashCombine(seed, FullName);
		XnaHHashCombine(seed, IsInterface);
		XnaHHashCombine(seed, IsArray);
		XnaHHashCombine(seed, IsPointer);
		XnaHHashCombine(seed, IsClass);
		XnaHHashCombine(seed, IsCOMObject);
		XnaHHashCombine(seed, IsEnum);
		XnaHHashCombine(seed, IsValueType);

		return seed;
	}
}