#include "xna/content/lzx/decoder.hpp"
//#include "libmspack/mspack.h"
//#include "libmspack/lzx.h"

namespace xna {
	LzxDecoder::LzxDecoder(int window) {
		if (window < 15 || window > 21)
			return;

		window_bits = window;
	}

	int LzxDecoder::Decompress(Stream* inData, int inLen, Stream* outData, int outLen) {
		/*mspack_file* input = nullptr;
		mspack_file* output = nullptr;

		auto lzxstream = lzxd_init(
			struct mspack_system* system,
			nullptr,
			struct mspack_file* input,
			input + inData->Position(),
			struct mspack_file* output,
			output + outData->Position(),
			int window_bits,
			window_bits,
			int reset_interval,
			0,
			int input_buffer_size,
			inLen,
			off_t output_length,
			outLen,
			char is_delta
			0
		);

		auto result = lzxd_decompress(
			struct lzxd_stream* lzx,
			lzxstream,
			off_t out_bytes
			0
		);*/

		return 0;
	}
}