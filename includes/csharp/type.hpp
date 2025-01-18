#ifndef CSHARP_TYPE_HPP
#define CSHARP_TYPE_HPP

#include <string>
#include <optional>
#include <memory>
#include <type_traits>
#include "misc.hpp"
#include <vector>
#include <map>
#include <optional>

namespace csharp {
	class Type {
	public:		
		constexpr std::string FullName() const { return fullname; }

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
		
		std::string fullname;

		size_t hashCode{ 0 };
		int flags{ 0 };

		template <class T>
		static constexpr Type FromTemplate();

		template <class T>
		friend Type typeof();

		/*template <class T>
		friend Type GetType(T value);	*/

		template <class T>
		friend Type GetType(T const& value);

		constexpr void InternalGenerateHashCode() {
			size_t seed = 0;
			misc::HashCombine(seed, fullname);

			hashCode = seed;
		}
	};				

	template <class T>
	constexpr Type Type::FromTemplate() {
		auto type = Type();
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

		return type;
	}	

	template <class T>
	Type typeof() {
		return Type::FromTemplate<T>();		
	}

	/*template <class T>
	Type GetType(T value) {
		return  Type::FromTemplate<T>();
	}*/

	template <class T>
	Type GetType(T const& value) {
		return  Type::FromTemplate<T>();
	}

	class RuntimeType {
	public:
		static inline void Add(Type const& type) {
			runtimeMap.insert({ type.FullName(), type });
		}

		static inline void Add(std::string const& typeName, Type const& type) {
			runtimeMap.insert({ typeName, type });
		}

		static inline std::unique_ptr<Type> GetType(std::string const& typeName) {
			const auto iterator = runtimeMap.find(typeName);

			if (iterator != runtimeMap.end())
				return std::make_unique<Type>(runtimeMap[typeName]);

			return nullptr;
		}

	private:
		static inline auto runtimeMap = std::map<std::string, Type>
		{
			{ typeid(char).name(), typeof<char>() },
			{ typeid(unsigned char).name(), typeof<unsigned char>() },
			{ typeid(short).name(), typeof<short>() },
			{ typeid(unsigned short).name(), typeof<unsigned short>() },
			{ typeid(int).name(), typeof<int>() },
			{ typeid(unsigned int).name(), typeof<unsigned int>() },
			{ typeid(long).name(), typeof<long>() },
			{ typeid(unsigned long).name(), typeof<unsigned long>() },
			{ typeid(long long).name(), typeof<long long>() },
			{ typeid(unsigned long long).name(), typeof<unsigned long long>() },
			{ typeid(float).name(), typeof<float>() },
			{ typeid(double).name(), typeof<double>() },
			{ typeid(std::string).name(), typeof<std::string>() },
		};		
	};
}

#endif