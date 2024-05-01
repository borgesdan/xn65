#include "object.hpp"
#include "type.hpp"

namespace xna {
	sptr<Type> Object::GetType()
	{
		auto type = New<Type>();
		type->FullName = "Object";
		type->Namespace = "xna";
		type->IsClass = true;
		return type;
	}
}