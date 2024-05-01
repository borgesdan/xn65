#ifndef XNA_CSHARP_BUFFER_HPP
#define XNA_CSHARP_BUFFER_HPP

#include <string>
#include <vector>

namespace xna {
	class Buffer {
	public:
		template <typename T>
		static void BlockCopy(T const* src, rsize_t srcOffset, T* dst, rsize_t dstOffset, rsize_t byteCount) {
			memmove_s(dst + dstOffset, byteCount, src + srcOffset, byteCount);
		}		

	private:
		constexpr Buffer() = default;
		constexpr Buffer(Buffer&&) = default;
		constexpr Buffer(const Buffer&) = default;
	};
}

#endif