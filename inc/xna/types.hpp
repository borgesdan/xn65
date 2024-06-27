#ifndef XNA_TYPES_HPP
#define XNA_TYPES_HPP

#include <cstdint>
#include <limits>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <cassert>
#include <optional>

namespace xna {

	//
	// C# standard types
	//

	using Sbyte		= int8_t;
	using Byte		= uint8_t;
	using Short		= int16_t;
	using Ushort	= uint16_t;
	using Int		= int32_t;
	using Uint		= uint32_t;
	using Long		= int64_t;
	using Ulong		= uint64_t;
	using Char		= char16_t;

	//
	// C# Min and Max Value
	//

	constexpr Sbyte		SbyteMaxValue	= (std::numeric_limits<Sbyte>::max)();
	constexpr Sbyte		SbyteMinValue	= (std::numeric_limits<Sbyte>::min)();
	constexpr Byte		ByteMaxValue	= (std::numeric_limits<Byte>::max)();
	constexpr Byte		ByteMinValue	= (std::numeric_limits<Byte>::min)();
	constexpr Short		ShortMaxValue	= (std::numeric_limits<Short>::max)();
	constexpr Short		ShortMinValue	= (std::numeric_limits<Short>::min)();
	constexpr Ushort	UshortMaxValue	= (std::numeric_limits<Ushort>::max)();
	constexpr Ushort	UshortMinValue	= (std::numeric_limits<Ushort>::min)();
	constexpr Int		IntMaxValue		= (std::numeric_limits<Int>::max)();
	constexpr Int		IntMinValue		= (std::numeric_limits<Int>::min)();
	constexpr Uint		UintMaxValue	= (std::numeric_limits<Uint>::max)();
	constexpr Uint		UintMinValue	= (std::numeric_limits<Uint>::min)();
	constexpr Long		LongMaxValue	= (std::numeric_limits<Long>::max)();
	constexpr Long		LongMinValue	= (std::numeric_limits<Long>::min)();
	constexpr Ulong		UlongMaxValue	= (std::numeric_limits<Ulong>::max)();
	constexpr Ulong		UlongMinValue	= (std::numeric_limits<Ulong>::min)();
	constexpr Char		CharMaxValue	= (std::numeric_limits<Char>::max)();
	constexpr Char		CharMinValue	= (std::numeric_limits<Char>::min)();
	constexpr float     FloatMaxValue	= (std::numeric_limits<float>::max)();
	constexpr float     FloatMinValue	= (std::numeric_limits<float>::min)();
	constexpr double    DoubleMaxValue	= (std::numeric_limits<double>::max)();
	constexpr double    DoubleMinValue	= (std::numeric_limits<double>::min)();

	//
	// About strings: https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
	//

	//Same as std::string
	using String	= std::string;

	//Same as std::wstring
	using WString	= std::wstring;	
	
	//Same as std::shared_ptr
	template <typename T>
	using sptr		= std::shared_ptr<T>;

	//Same as std::weak_ptr
	template <typename T>
	using wptr = std::weak_ptr<T>;

	//Same as std::unique_ptr
	template <typename T>
	using uptr = std::unique_ptr<T>;		
		
	//Same as std::make_shared
	template <class _Ty, class... _Types>
	inline std::shared_ptr<_Ty> snew(_Types&&... _Args) {
		return std::make_shared<_Ty>(std::forward<_Types>(_Args)...);
	}

	//Same as std::make_unique
	template <class _Ty, class... _Types>
	inline std::unique_ptr<_Ty> unew(_Types&&... _Args) {
		return std::make_unique<_Ty>(std::forward<_Types>(_Args)...);
	}

//See ref: https://en.cppreference.com/w/cpp/error/assert
#define assertm(exp, msg) assert(((void)msg, exp))
}

#endif