#ifndef XNA_CONTENT_LZX_DECODERSTREAM_HPP
#define XNA_CONTENT_LZX_DECODERSTREAM_HPP

#include "decoder.hpp"

namespace xna {
	class LzxDecoderStream : public Stream {
	public:
		LzxDecoderStream(sptr<Stream>& input, Int decompressedSize, Int compressedSize) {
			dec = New<LzxDecoder>(16);
            Decompress(input, decompressedSize, compressedSize);
		}

	private:
		void Decompress(sptr<Stream>& stream, Int decompressedSize, Int compressedSize) {
            //thanks to ShinAli (https://bitbucket.org/alisci01/xnbdecompressor)
           // default window size for XNB encoded files is 64Kb (need 16 bits to represent it)
            decompressedStream = New<MemoryStream>(decompressedSize);
            auto startPos = stream->Position();
            auto pos = startPos;

            while (pos - startPos < compressedSize)
            {
                // the compressed stream is seperated into blocks that will decompress
                // into 32Kb or some other size if specified.
                // normal, 32Kb output blocks will have a short indicating the size
                // of the block before the block starts
                // blocks that have a defined output will be preceded by a byte of value
                // 0xFF (255), then a short indicating the output size and another
                // for the block size
                // all shorts for these cases are encoded in big endian order
                Int hi = stream->ReadByte();
                Int lo = stream->ReadByte();
                Int block_size = (hi << 8) | lo;
                Int frame_size = 0x8000; // frame size is 32Kb by default
                // does this block define a frame size?
                
                if (hi == 0xFF) {
                    hi = lo;
                    lo = static_cast<Byte>(stream->ReadByte());
                    frame_size = (hi << 8) | lo;
                    hi = static_cast<Byte>(stream->ReadByte());
                    lo = static_cast<Byte>(stream->ReadByte());
                    block_size = (hi << 8) | lo;
                    pos += 5;
                }
                else
                    pos += 2;

                // either says there is nothing to decode
                if (block_size == 0 || frame_size == 0)
                    break;

                auto decompressed = reinterpret_pointer_cast<Stream>(decompressedStream);
                dec->Decompress(stream, block_size, decompressed, frame_size);
                pos += block_size;

                // reset the position of the input just incase the bit buffer
                // read in some unused bytes
                stream->Seek(pos, SeekOrigin::Begin);
            }

            if (decompressedStream->Position() != decompressedSize)
            {
                return;
            }

            decompressedStream->Seek(0, SeekOrigin::Begin);
		}
	private:
		sptr<LzxDecoder> dec = nullptr;
		sptr<MemoryStream>decompressedStream = nullptr;

        // Inherited via Stream
        Int Length() override;
        Long Position() override;
        void Close() override;
        Long Seek(Long offset, SeekOrigin const& origin) override;
        Int Read(Byte* buffer, Int bufferLength, Int offset, Int count) override;
        Int Read(std::vector<Byte>& buffer, Int offset, Int count) override;
        Int ReadByte() override;
        void Write(Byte const* buffer, Int bufferLength, Int offset, Int count) override;
        void Write(std::vector<Byte> const& buffer, Int offset, Int count) override;
        void WriteByte(Byte value) override;
        virtual constexpr bool IsClosed() override { return false; }
    };
}

#endif