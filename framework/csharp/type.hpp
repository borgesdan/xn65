#ifndef XNA_CSHARP_TYPE_HPP
#define XNA_CSHARP_TYPE_HPP

#include "../default.hpp"
#include "object.hpp"

namespace xna {
	class Type {
	public:
		String Namespace;
		String FullName;
		bool IsInterface{ false };
		bool IsArray{ false };
		bool IsPointer{ false };
		bool IsClass{ false };
		bool IsCOMObject{ false };
		bool IsEnum{ false };
		bool IsValueType{ false };
	};

	template <class T>
	inline sptr<Type> typeof(T const& obj) {
		auto obj = reinterpret_cast<Object*>(&obj);

		if (!obj) 
			return nullptr;

		return obj->GetType();
	}
}

#endif