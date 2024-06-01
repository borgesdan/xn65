#ifndef XNA_HELPERS_HPP
#define XNA_HELPERS_HPP

#include <string>
#include <utility>

namespace xna {
	inline std::wstring XnaHToWString(const std::string& str)
	{
		std::wstring wstr;
		size_t size;
		wstr.resize(str.length());
		mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
		return wstr;
	}

	inline std::string XnaHToString(const std::wstring& wstr)
	{
		std::string str;
		size_t size;
		str.resize(wstr.length());
		wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
		return str;
	}

	template <class T>
	constexpr void XnaHHashCombine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template<typename T> struct is_shared_ptr : std::false_type {};
	template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

	template<typename T>
	inline auto ReturnDefaultOrNull() {
		if constexpr (is_shared_ptr<T>::value)
			return (T)nullptr;
		else
			return T();
	}

	/*template<typename T>
	inline auto ReturnAuto(T& value) {
		return value;
	}*/
}

#endif