#ifndef CSHARP_TEXT_UNICODE_HPP
#define CSHARP_TEXT_UNICODE_HPP

#include <cstdint>
#include <vector>
#include "csharp/exception.hpp"

//
// The char type keyword is an alias for the .NET System.Char structure type that represents a Unicode UTF-16 character.
// https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/char
//

namespace csharp {
	struct UnicodeUtility {
		// The Unicode replacement character U+FFFD.
		inline static uint32_t ReplacementChar = 0xFFFD;

		// Returns the Unicode plane (0 through 16, inclusive) which contains this code point.
		static constexpr int32_t GetPlane(uint32_t codePoint) {
			return static_cast<int32_t>(codePoint >> 16);
		}

		// Returns a Unicode scalar value from two code points representing a UTF-16 surrogate pair.
		static constexpr uint32_t GetScalarFromUtf16SurrogatePair(uint32_t highSurrogateCodePoint, uint32_t lowSurrogateCodePoint) {
			return (highSurrogateCodePoint << 10) + lowSurrogateCodePoint - ((0xD800U << 10) + 0xDC00U - (1 << 16));
		}

		// Given a Unicode scalar value, gets the number of UTF-16 code units required to represent this value.
		static constexpr int32_t GetUtf16SequenceLength(uint32_t value) {
			value -= 0x10000;
			value += (2 << 24);
			value >>= 24;
			return static_cast<int32_t>(value);
		}

		// Decomposes an astral Unicode scalar into UTF-16 high and low surrogate code units.
		static constexpr void GetUtf16SurrogatesFromSupplementaryPlaneScalar(uint32_t value, char16_t& highSurrogateCodePoint, char16_t& lowSurrogateCodePoint) {
			highSurrogateCodePoint = static_cast<char16_t>((value + ((0xD800u - 0x40u) << 10)) >> 10);
			lowSurrogateCodePoint = static_cast<char16_t>((value & 0x3FFu) + 0xDC00u);
		}

		// Given a Unicode scalar value, gets the number of UTF-8 code units required to represent this value.
		static constexpr int32_t GetUtf8SequenceLength(uint32_t value) {
			int32_t a = (static_cast<int32_t>(value) - 0x0800) >> 31;

			value ^= 0xF800u;
			value -= 0xF880u;
			value += (4 << 24);
			value >>= 24;

			return static_cast<int32_t>(value) + (a * 2);
		}

		// Returns true if is an ASCII
		static constexpr bool IsAsciiCodePoint(uint32_t value) {
			return value <= 0x7Fu;
		}

		//Returns true if is a UTF-16 high surrogate code point, i.e., is in [ U+D800..U+DBFF ], inclusive.
		static constexpr bool IsHighSurrogateCodePoint(uint32_t value) {
			return IsInRangeInclusive(value, 0xD800U, 0xDBFFU);
		}
		
		//Returns true if is between
		static constexpr bool IsInRangeInclusive(uint32_t value, uint32_t lowerBound, uint32_t upperBound) {
			return (value - lowerBound) <= (upperBound - lowerBound);
		}

		//Returns true if value is a UTF-16 low surrogate code point, i.e., is in [ U+DC00..U+DFFF ], inclusive.
		static constexpr bool IsLowSurrogateCodePoint(uint32_t value) {
			return IsInRangeInclusive(value, 0xDC00U, 0xDFFFU);
		}

		//Returns true if value is a UTF-16 low surrogate code point, i.e., is in [ U+D800..U+DFFF ], inclusive.
		static constexpr bool IsSurrogateCodePoint(uint32_t value) {
			return IsInRangeInclusive(value, 0xD800U, 0xDFFFU);
		}
		 
		//Returns true if value is a valid Unicode code point, i.e., is in [ U+0000..U+10FFFF ], inclusive.
		static constexpr bool IsValidCodePoint(uint32_t codePoint) {
			return codePoint <= 0x10FFFFU;
		}

		//Returns true if value is a valid Unicode scalar value, i.e., is in [ U+0000..U+D7FF ], inclusive; or [ U+E000..U+10FFFF ], inclusive.
		static constexpr bool IsValidUnicodeScalar(uint32_t value) {
			return ((value - 0x110000u) ^ 0xD800u) >= 0xFFEF0800u;
		}
	};
}

#endif