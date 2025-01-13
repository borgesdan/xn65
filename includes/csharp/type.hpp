#ifndef CSHARP_TYPE_HPP
#define CSHARP_TYPE_HPP

#include <string>
#include <optional>
#include <memory>
#include <type_traits>
#include "misc.hpp"

namespace csharp {
	class Type {
	public:
		constexpr std::string Namespace() const { return ns; }
		constexpr std::string FullName() const { return fullname; }
		constexpr std::string Name() const { return name; }

		constexpr bool IsInterface() const { return isInterface; }
		constexpr bool IsArray() const { return isArray; }
		constexpr bool IsPointer() const { return isPointer; }
		constexpr bool IsAbstract() const { return isAbstract; }
		constexpr bool IsClass() const { return isClass; }
		constexpr bool IsEnum() const { return isEnum; }
		constexpr bool IsPrimitive() const { return isPrimitive; }
		constexpr bool IsValueType() const { return isValueType; }		

		constexpr size_t GetHashCode() const {
			return hashCode;
		}

	public:
		constexpr Type() = default;

	private:
		enum class TypeFlags {
			Interface = 1,
			Array = 2,
			Pointer = 4,
			Class = 8,
			Enum = 16,
			Primitive = 32,
			ValueType = 64
		};

		std::string ns;
		std::string fullname;
		std::string name;

		bool isInterface{ false };
		bool isArray{ false };
		bool isPointer{ false };
		bool isAbstract{ false };
		bool isClass{ false };
		bool isEnum{ false };
		bool isPrimitive{ false };
		bool isValueType{ false };		

		size_t hashCode{ 0 };

	public:
		template <class T>
		friend std::unique_ptr<Type> typeof();

		template <class T>
		friend constexpr void typeof(Type& type);

		template <class T>
		friend std::unique_ptr<Type> GetType(T const& value);

		template <class T>
		friend constexpr void GetType(T const& value, Type& type);

		constexpr void InternalGenerateHashCode() {
			size_t seed = 0;
			misc::HashCombine(seed, fullname);

			hashCode = seed;
		}

	private:
		template <class T>
		static constexpr void FromTemplate(Type& type);
	};	

	template <class T>
	inline std::unique_ptr<Type> typeof() {
		Type t;
		Type::FromTemplate<T>(t);

		return std::make_unique<Type>(t);
	}

	template <class T>
	constexpr void typeof(Type& t) {
		Type::FromTemplate<T>(t);
	}

	template <class T>
	inline std::unique_ptr<Type> GetType(T const& value) {
		return typeof<T>();
	}

	template <class T>
	constexpr void GetType(T const& value, Type& type) {
		typeof<T>(type);
	}

	template <class T>
	constexpr void Type::FromTemplate(Type& type) {
		type = Type();
		std::string fullname = typeid(T).name();
		type.fullname = fullname;

		//An integral type or a floating-point type
		if constexpr (std::is_arithmetic<T>::value) {
			type.isPrimitive = true;
			type.isValueType = true;
		}
		//checks whether T is an enumeration type.
		else if constexpr (std::is_enum<T>::value) {
			type.isValueType = true;
			type.isEnum = true;			
		}
		//Checks whether T is a pointer to object or function (including pointer to void, but excluding pointer to member)
		//or a cv-qualified version thereof.
		else if constexpr (std::is_pointer<T>::value || misc::IsSmartPointer<T>()) {
			type.isPointer = true;			
		}
		//Checks whether T is a non-union class type. 
		else if constexpr (std::is_class<T>::value) {
			type.isClass = true;			
		}
		else if constexpr (std::is_abstract<T>::value) {
			type.isInterface = true;			
		}
		else if constexpr (std::is_array<T>::value) {
			type.isArray = true;			
		}

		type.InternalGenerateHashCode();
	}
}

#endif