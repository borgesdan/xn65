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

		constexpr bool IsInterface() const { return misc::HasFlag(flags, TypeFlags::Interface); }
		constexpr bool IsArray() const { return misc::HasFlag(flags, TypeFlags::Array); }
		constexpr bool IsPointer() const { return misc::HasFlag(flags, TypeFlags::Pointer); }
		constexpr bool IsClass() const { return misc::HasFlag(flags, TypeFlags::Class); }
		constexpr bool IsEnum() const { return misc::HasFlag(flags, TypeFlags::Enum); }
		constexpr bool IsPrimitive() const { return misc::HasFlag(flags, TypeFlags::Primitive); }
		constexpr bool IsValueType() const { return misc::HasFlag(flags, TypeFlags::ValueType); }
		constexpr bool IsSmartPointer() const { return misc::HasFlag(flags, TypeFlags::SmartPointer); }

		constexpr size_t GetHashCode() const {
			return hashCode;
		}

	public:
		constexpr Type() = default;

	private:
		enum class TypeFlags {
			Interface = 1 << 0,
			Array = 1 << 1,//2
			Pointer = 1 << 2,
			Class = 1 << 3, //8
			Enum = 1 << 4,
			Primitive = 1 << 5, //32
			ValueType = 1 << 6,
			SmartPointer = 1 << 7, //128
		};

		std::string ns;
		std::string fullname;
		std::string name;

		size_t hashCode{ 0 };
		int flags{ 0 };

	public:
		template <class T>
		friend std::unique_ptr<Type> typeof();

		template <class T>
		friend constexpr void typeof(Type& type);

		template <class T>
		friend constexpr Type typeof_v();

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
	constexpr Type typeof_v() {
		Type t;
		Type::FromTemplate<T>(t);

		return t;
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
			misc::SetFlag(type.flags, TypeFlags::Primitive, TypeFlags::ValueType);
		}
		//checks whether T is an enumeration type.
		else if constexpr (std::is_enum<T>::value) {
			misc::SetFlag(type.flags, TypeFlags::Enum, TypeFlags::ValueType);
		}
		//Checks whether T is a pointer to object or function (including pointer to void, but excluding pointer to member)
		//or a cv-qualified version thereof.
		else if constexpr (std::is_pointer<T>::value || misc::IsSmartPointer<T>()) {
			misc::SetFlag(type.flags, TypeFlags::Pointer);

			if constexpr (misc::IsSmartPointer<T>())
				misc::SetFlag(type.flags, TypeFlags::SmartPointer);
		}
		//Checks whether T is a non-union class type. 
		else if constexpr (std::is_class<T>::value) {
			misc::SetFlag(type.flags, TypeFlags::Class);
		}
		else if constexpr (std::is_abstract<T>::value) {
			misc::SetFlag(type.flags, TypeFlags::Interface);
		}
		else if constexpr (std::is_array<T>::value) {
			misc::SetFlag(type.flags, TypeFlags::Array);
		}

		type.InternalGenerateHashCode();
	}
}

#endif