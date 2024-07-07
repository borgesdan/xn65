#ifndef XNA_HELPERS_HPP
#define XNA_HELPERS_HPP

#include <string>
#include <utility>
#include "exception.hpp"

namespace xna {
	//Class for helper functions
	struct XnaHelper {

		//
		// Smart Pointer Comparator
		//

		template<typename T> struct is_shared_ptr : std::false_type {};
		template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
		template<typename T> struct is_weak_ptr : std::false_type {};
		template<typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
		template<typename T> struct is_unique_ptr : std::false_type {};
		template<typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

		//Returns true if the type is a smart pointer
		template <typename T>
		static constexpr bool IsSmartPoint() {
			return is_shared_ptr<T>::value || is_unique_ptr<T>::value || is_weak_ptr<T>::value;
		}

		//Convert a string to wstring
		static inline std::wstring ToWString(const std::string& str)
		{
			std::wstring wstr;
			size_t size;
			wstr.resize(str.length());
			mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
			return wstr;
		}

		//Convert a wstring to string
		static inline std::string ToString(const std::wstring& wstr)
		{
			std::string str;
			size_t size;
			str.resize(wstr.length());
			wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
			return str;
		}

		//Returns a hash reporting input values
		template <class T>
		static constexpr void HashCombine(std::size_t& seed, const T& v) {
			std::hash<T> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}		

		//Returns null if the type is a smart pointer or default value if the type has a default constructor.
		//Throws an exception if the object cannot be created
		template<typename T>
		static inline auto ReturnDefaultOrNull(const std::source_location location = std::source_location::current()) {
			if constexpr (IsSmartPoint<T>())
				return (T)nullptr;
			else if constexpr (std::is_default_constructible<T>::value)
				return T();
			else
				Exception::Throw(Exception::UNABLE_TO_BUILD_OBJECT, location);
		}
	};		
}

#endif