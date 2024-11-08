#ifndef XNA_CSHARP_BUFFER_HPP
#define XNA_CSHARP_BUFFER_HPP

#include <string>
#include <vector>

namespace xna {
	//A simplified port of the System.Buffer class.
	class Buffer {
	public:
		//Copies from one primitive array to another primitive array without respecting types.
		template <typename T>
		static inline void BlockCopy(T const* src, rsize_t srcOffset, T* dst, rsize_t dstOffset, rsize_t byteCount) {
			memmove_s(dst + dstOffset, byteCount, src + srcOffset, byteCount);
		}

		//Copies from one primitive array to another primitive array without respecting types.
		template <typename TSOURCE, typename TDEST>
		static inline void BlockCopy(TSOURCE const* src, rsize_t srcOffset, TDEST* dst, rsize_t dstOffset, rsize_t byteCount) {
			memmove_s(dst + dstOffset, byteCount, src + srcOffset, byteCount);
		}

	private:
		constexpr Buffer() = default;
		constexpr Buffer(Buffer&&) = default;
		constexpr Buffer(const Buffer&) = default;
	};
}

#endif