#ifndef XNA_CSHARP_TYPE_HPP
#define XNA_CSHARP_TYPE_HPP

#include "misc.hpp"
#include "../exception.hpp"
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace xna {
	class Type {
	public:
		constexpr std::string FullName() const { return fullName; }
		constexpr bool IsClass() const { return isClass; }
		constexpr bool IsEnum() const { return isEnum; }
		constexpr bool IsValueType() const { return isValueType; }
		constexpr bool IsPrimitive() const { return isPrimitive; }
		constexpr bool IsPointer() const { return isPointer; }

		constexpr size_t GetHashCode() const {
			size_t seed = 0;
			misc::HashCombine(seed, fullName);
			misc::HashCombine(seed, isClass);
			misc::HashCombine(seed, isEnum);
			misc::HashCombine(seed, isValueType);
			misc::HashCombine(seed, isPrimitive);

			return seed;
		}

		constexpr bool operator==(const Type& other) const {
			return
				fullName == other.fullName
				&& isClass == other.isClass
				&& isEnum == other.isEnum
				&& isValueType == other.isValueType
				&& isPointer == other.isPointer
				&& isPrimitive == other.isPrimitive;
		}

		bool operator()(Type const& t1, Type const& t2) const {
			return t1.GetHashCode() < t2.GetHashCode();
		}

		template <class T>
		friend std::shared_ptr<Type> typeof();

	public:
		inline static auto NameOfRegisteredTypes = std::map<std::string, std::shared_ptr<Type>>();

	private:
		std::string fullName;						
		bool isClass{ false };
		bool isEnum{ false };
		bool isValueType{ false };
		bool isPrimitive{ false };	
		bool isPointer { false };	
	};

	template <class T>
	inline std::shared_ptr<Type> typeof() {

		auto type = std::make_shared<Type>();
		type->fullName = typeid(T).name();

		if (std::is_arithmetic<T>::value) {									
			type->isPrimitive = true;
			type->isValueType = true;			
		}
		else if (std::is_enum<T>::value) {
			type->isValueType = true;
			type->isEnum = true;			
		}
		else if (std::is_pointer<T>::value) {
			type->isPointer = true;			
		}
		else if (std::is_class<T>::value) {
			type->isClass = true;
		}
		else 
		{
			Exception::Throw(Exception::INVALID_OPERATION);
		}
		
		return type;
	}	

	template <class T>
	inline std::shared_ptr<Type> typeof(T const* object) {
		return typeof<T>();
	}

	template <class T>
	inline std::shared_ptr<Type> typeof(T const& object) {
		return typeof<T>();
	}
}

#endif