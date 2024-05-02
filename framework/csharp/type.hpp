#ifndef XNA_CSHARP_TYPE_HPP
#define XNA_CSHARP_TYPE_HPP

#include "../default.hpp"
#include "object.hpp"

namespace xna {
	class Type : public Object {
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

		constexpr bool operator==(const Type& other) const
		{
			return Namespace == other.Namespace 
				&& FullName == other.FullName 
				&& IsInterface == other.IsInterface 
				&& IsArray == other.IsArray 
				&& IsPointer == other.IsPointer 
				&& IsClass == other.IsClass 
				&& IsCOMObject == other.IsCOMObject 
				&& IsEnum == other.IsEnum 
				&& IsValueType == other.IsValueType;
		}

		virtual sptr<Type> GetType() const override;
		virtual size_t GetHashCode() const;

		bool operator()(Type const& t1, Type const& t2) const {
			return t1.GetHashCode() < t2.GetHashCode();
		}
	};	

	template <class T>
	inline sptr<Type> typeof() {
		auto t = New<T>();
		auto obj = reinterpret_pointer_cast<Object>(t);

		if (!obj) return nullptr;
		
		return  obj->GetType();
	}

	template <class T>
	inline sptr<Type> typeof(T const* object) {
		auto obj = reinterpret_cast<const Object*>(object);

		if (!obj)
			return nullptr;

		return obj->GetType();
	}
}

#endif