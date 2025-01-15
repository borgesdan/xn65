#ifndef CSHARP_ACTIVATOR_HPP
#define CSHARP_ACTIVATOR_HPP

#include <any>
#include <cstdint>
#include <string>
#include "type.hpp"
#include "optional"
#include <utility>

namespace csharp {
	class Activator {
	public:
		template <typename... Arguments>
		static std::any CreateInstance(Type const& type, Arguments &&... args) {
			const auto typeHashCode = type.GetHashCode();
			std::any obj;

			if (InternalCreateKnowTypes(typeHashCode, obj))
				return obj;

			return obj;
		}

	private:		
		template <typename... Arguments>
		static constexpr bool InternalCreateKnowTypes(size_t const& typeHashCode, std::any& obj, Arguments &&... args) {

			//auto a = std::any(std::in_place_type<int8_t>, std::forward<Arguments>(args)...);
			
			if (typeHashCode == int8Hash)
				obj = std::any(std::in_place_type<int8_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == int16Hash)
				obj = std::any(std::in_place_type<int16_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == int32Hash)
				obj = std::any(std::in_place_type<int32_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == int64Hash)
				obj = std::any(std::in_place_type<int64_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == floatHash)
				obj = std::any(std::in_place_type<float>, std::forward<Arguments>(args)...);
			else if (typeHashCode == doubleHash)
				obj = std::any(std::in_place_type<double>, std::forward<Arguments>(args)...);
			else if (typeHashCode == uint8Hash)
				obj = std::any(std::in_place_type<uint8_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == uint16Hash)
				obj = std::any(std::in_place_type<uint16_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == uint32Hash)
				obj = std::any(std::in_place_type<uint32_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == uint64Hash)
				obj = std::any(std::in_place_type<uint64_t>, std::forward<Arguments>(args)...);
			else if (typeHashCode == charHash)
				obj = std::any(std::in_place_type<char>, std::forward<Arguments>(args)...);
			else if (typeHashCode == stringHash)
				obj = std::any(std::in_place_type<std::string>, std::forward<Arguments>(args)...);
			else
				return false;

			return true;
		}

	private:			
		static inline const size_t int8Hash = typeof_v<int8_t>().GetHashCode();
		static inline const size_t int16Hash = typeof_v<int16_t>().GetHashCode();
		static inline const size_t int32Hash = typeof_v<int32_t>().GetHashCode();
		static inline const size_t int64Hash = typeof_v<int64_t>().GetHashCode();
		static inline const size_t floatHash = typeof_v<float>().GetHashCode();
		static inline const size_t doubleHash = typeof_v<double>().GetHashCode();
		static inline const size_t uint8Hash = typeof_v<uint8_t>().GetHashCode();
		static inline const size_t uint16Hash = typeof_v<uint16_t>().GetHashCode();
		static inline const size_t uint32Hash = typeof_v<uint32_t>().GetHashCode();
		static inline const size_t uint64Hash = typeof_v<uint64_t>().GetHashCode();
		static inline const size_t stringHash = typeof_v<std::string>().GetHashCode();
		static inline const size_t charHash = typeof_v<char>().GetHashCode();
	};
}

#endif