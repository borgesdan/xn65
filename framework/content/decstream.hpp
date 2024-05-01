#ifndef XNA_CONTENT_DECOMPRESS_STREAM_HPP
#define XNA_CONTENT_DECOMPRESS_STREAM_HPP

#include "../default.hpp"
#include "../csharp/stream.hpp"
#include <mspack.h>

namespace xna {
	class DecompressStream : public Stream {
	public:
		DecompressStream(sptr<Stream> const& baseStream, Int compressedTodo, Int decompressedTodo):
		baseStream(baseStream), compressedTodo(compressedTodo), decompressedTodo(decompressedTodo){
			compressedBuffer = std::vector<Byte>(CompressedBufferSize);
			decompressedBuffer = std::vector<Byte>(DecompressedBufferSize);
			decompressionContext = mspack_create_cab_decompressor(nullptr);
		}

		virtual ~DecompressStream() {
			if (decompressionContext) {
				mspack_destroy_cab_decompressor(decompressionContext);
			}
		}

		void Decompress();

	public:
		// Inherited via Stream
		Int Length() override;
		Long Position() override;
		void Close() override;
		Long Seek(Long offset, SeekOrigin const& origin, xna_error_nullarg) override;
		Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_nullarg) override;
		Int ReadByte(xna_error_nullarg) override;
		void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_nullarg) override;
		void WriteByte(Byte value, xna_error_nullarg) override;

	private:
		static constexpr int CompressedBufferSize = 65536;
		static constexpr int DecompressedBufferSize = 65536;
		sptr<Stream> baseStream = nullptr;
		Int compressedTodo{ 0 };
		Int compressedSize{ 0 };
		Int compressedPosition{ 0 };
		std::vector<Byte> compressedBuffer;
		Int decompressedTodo{ 0 };
		Int decompressedSize{ 0 };
		Int decompressedPosition{ 0 };
		std::vector<Byte> decompressedBuffer;
		mscab_decompressor* decompressionContext = nullptr;

	private:
		bool DecompressNextBuffer();
		void ReadNextBufferFromDisk();
		void ReadBufferFromDisk(std::vector<Byte>& buffer, Int& bufferTodo, Int& bufferSize);
	};
}

#endif