#ifndef XNA_CONTENT_LZX_LZXDECODE_HPP
#define XNA_CONTENT_LZX_LZXDECODE_HPP

#include "../../default.hpp"
#include "../../csharp/stream.hpp"
#include <algorithm>

namespace xna {
	struct LzxDecoder {
		LzxDecoder(int window);
		int Decompress(Stream* inData, int inLen, Stream* outData, int outLen);
		int window_bits = 0;
	};
}

#endif