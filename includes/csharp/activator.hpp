#ifndef CSHARP_ACTIVATOR_HPP
#define CSHARP_ACTIVATOR_HPP

#include <any>
#include <cstdint>
#include <string>
#include "type.hpp"
#include <optional>
#include <utility>
#include <vector>
#include <functional>
#include <memory>
#include "csharp/drawing/primitives.hpp"

namespace csharp {
	class Activator {
	public:
		template <typename... Arguments>
		static std::any CreateInstance(Type const& type, Arguments &&... args) {
			const auto typeHashCode = type.GetHashCode();
			std::any obj;

			InternalCreateKnowTypes(typeHashCode, obj));

			return obj;
		}

		template <class... Arguments>
		using  func = std::function<bool(Type const&, std::any&, Arguments &&...)>;

		template <typename... Arguments>
		static std::any CreateInstance(Type const& type, std::function<bool(Type const&, std::any&, Arguments &&...)>& callback) {
			const auto typeHashCode = type.GetHashCode();
			std::any obj;

			if (callback) {
				callback(type, obj, std::forward<Arguments>(args)...);
			}

			return obj;
		}

		template <typename T, typename... Arguments>
		static std::any CreateInstance(Type_T<T> const& type, Arguments &&... args) {			
			using value_type = Type_T<T>::value_type;

			std::any obj = std::any(std::in_place_type<value_type>, std::forward<Arguments>(args)...);

			return obj;
		}	
		

	private:		
		template <typename... Arguments>
		static constexpr bool InternalCreateKnowTypes(size_t const& typeHashCode, std::any& obj, Arguments &&... args) {			
			if (typeHashCode == charHash)
				obj = std::any(std::in_place_type<char>, std::forward<Arguments>(args)...);
			else if (typeHashCode == shortHash)
				obj = std::any(std::in_place_type<short>, std::forward<Arguments>(args)...);
			else if (typeHashCode == intHash)
				obj = std::any(std::in_place_type<int>, std::forward<Arguments>(args)...);
			else if (typeHashCode == longHash)
				obj = std::any(std::in_place_type<long>, std::forward<Arguments>(args)...);
			else if (typeHashCode == long2Hash)
				obj = std::any(std::in_place_type<long long>, std::forward<Arguments>(args)...);
			else if (typeHashCode == ucharHash)
				obj = std::any(std::in_place_type<unsigned char>, std::forward<Arguments>(args)...);
			else if (typeHashCode == ushortHash)
				obj = std::any(std::in_place_type<unsigned short>, std::forward<Arguments>(args)...);
			else if (typeHashCode == uintHash)
				obj = std::any(std::in_place_type<unsigned int>, std::forward<Arguments>(args)...);
			else if (typeHashCode == ulongHash)
				obj = std::any(std::in_place_type<unsigned long>, std::forward<Arguments>(args)...);
			else if (typeHashCode == ulong2Hash)
				obj = std::any(std::in_place_type<unsigned long long>, std::forward<Arguments>(args)...);
			else if (typeHashCode == floatHash)
				obj = std::any(std::in_place_type<float>, std::forward<Arguments>(args)...);
			else if (typeHashCode == doubleHash)
				obj = std::any(std::in_place_type<double>, std::forward<Arguments>(args)...);
			else if (typeHashCode == stringHash)
				obj = std::any(std::in_place_type<std::string>, std::forward<Arguments>(args)...);
			else
				return false;

			return true;
		}

	private:			
		static inline const size_t charHash = typeof_v<char>().GetHashCode();
		static inline const size_t shortHash = typeof_v<short>().GetHashCode();
		static inline const size_t intHash = typeof_v<int>().GetHashCode();
		static inline const size_t longHash = typeof_v<long>().GetHashCode();
		static inline const size_t long2Hash = typeof_v<long long>().GetHashCode();
		static inline const size_t ucharHash = typeof_v<unsigned char>().GetHashCode();
		static inline const size_t ushortHash = typeof_v<unsigned short>().GetHashCode();
		static inline const size_t uintHash = typeof_v<unsigned int>().GetHashCode();
		static inline const size_t ulongHash = typeof_v<unsigned long>().GetHashCode();
		static inline const size_t ulong2Hash = typeof_v<unsigned long long>().GetHashCode();
		static inline const size_t floatHash = typeof_v<float>().GetHashCode();
		static inline const size_t doubleHash = typeof_v<double>().GetHashCode();
		static inline const size_t stringHash = typeof_v<std::string>().GetHashCode();		
	};
}

#endif