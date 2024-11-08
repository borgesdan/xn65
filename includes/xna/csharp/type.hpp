#ifndef XNA_CSHARP_TYPE_HPP
#define XNA_CSHARP_TYPE_HPP

#include "xna/helpers.hpp"
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
			XnaHelper::HashCombine(seed, fullName);
			XnaHelper::HashCombine(seed, isClass);
			XnaHelper::HashCombine(seed, isEnum);
			XnaHelper::HashCombine(seed, isValueType);
			XnaHelper::HashCombine(seed, isPrimitive);

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
		if (std::is_arithmetic<T>::value) {
			auto primitiveType = std::make_shared<Type>();			
			primitiveType->fullName = typeid(T).name();
			primitiveType->isPrimitive = true;
			primitiveType->isValueType = true;
			return primitiveType;
		}

		if (std::is_enum<T>::value) {
			auto enumType = std::make_shared<Type>();
			enumType->fullName = typeid(T).name();
			enumType->isValueType = true;
			enumType->isEnum = true;
			return enumType;
		}

		if (std::is_pointer<T>::value) {
			auto pointerType = std::make_shared<Type>();
			pointerType->fullName = typeid(T).name();
			pointerType->isPointer = true;
			return pointerType;
		}

		if (std::is_class<T>::value) {
			auto classType = std::make_shared<Type>();
			classType->fullName = typeid(T).name();
			classType->isClass = true;
			
			return classType;
		}
		
		return nullptr;
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