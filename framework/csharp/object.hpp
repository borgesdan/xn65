#ifndef XNA_CSHARP_OBJECT_HPP
#define XNA_CSHARP_OBJECT_HPP

#include "../default.hpp"

namespace xna {
	class Object {
	public:
		virtual sptr<Type> GetType() const;
		virtual size_t GetHashCode() const;
	};
}

#endif