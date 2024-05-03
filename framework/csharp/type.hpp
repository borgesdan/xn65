#ifndef XNA_CSHARP_TYPE_HPP
#define XNA_CSHARP_TYPE_HPP

#include "../default.hpp"
#include "object.hpp"
#include <type_traits>
#include <typeinfo>

namespace xna {
	class Type : public Object {
	public:
		constexpr String FullName() const { return fullName; }
		constexpr bool IsClass() const { return isClass; }
		constexpr bool IsEnum() const { return isEnum; }
		constexpr bool IsValueType() const { return isValueType; }
		constexpr bool IsPrimitive() const { return isPrimitive; }

		virtual size_t GetHashCode() const;

		constexpr bool operator==(const Type& other) const {
			return
				fullName == other.fullName
				&& isClass == other.isClass
				&& isEnum == other.isEnum
				&& isValueType == other.isValueType
				&& isPrimitive == other.isPrimitive;
		}

		bool operator()(Type const& t1, Type const& t2) const {
			return t1.GetHashCode() < t2.GetHashCode();
		}

		template <class T>
		friend sptr<Type> typeof();

	private:
		String fullName;						
		bool isClass{ false };
		bool isEnum{ false };
		bool isValueType{ false };
		bool isPrimitive{ false };				
	};

	template <class T>
	inline sptr<Type> typeof() {
		if (std::is_arithmetic<T>::value) {
			auto primitiveType = New<Type>();			
			primitiveType->fullName = typeid(T).name();
			primitiveType->isPrimitive = true;
			primitiveType->isValueType = true;
			return primitiveType;
		}

		if (std::is_enum<T>::value) {
			auto enumType = New<Type>();
			enumType->fullName = typeid(T).name();
			enumType->isValueType = true;
			enumType->isEnum = true;
			return enumType;
		}

		if (std::is_class<T>::value) {
			auto classType = New<Type>();
			classType->fullName = typeid(T).name();
			classType->isClass = true;
			
			return classType;
		}

		return nullptr;
	}	

	template <class T>
	inline sptr<Type> typeof(T const* object) {
		return typeof<T>();
	}

	template <class T>
	inline sptr<Type> typeof(T const& object) {
		return typeof<T>();
	}
}

#endif