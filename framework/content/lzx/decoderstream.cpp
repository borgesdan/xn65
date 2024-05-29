#include "content/lzx/decoderstream.hpp"

namespace xna {
	Int LzxDecoderStream::Length()
	{
		return 0;
	}
	Long LzxDecoderStream::Position()
	{
		return 0;
	}
	void LzxDecoderStream::Close()
	{
	}
	Long LzxDecoderStream::Seek(Long offset, SeekOrigin const& origin)
	{
		return Long();
	}
	Int LzxDecoderStream::Read(Byte* buffer, Int bufferLength, Int offset, Int count)
	{
		return decompressedStream->Read(buffer, bufferLength, offset, count);
	}
	Int LzxDecoderStream::Read(std::vector<Byte>& buffer, Int offset, Int count)
	{
		return decompressedStream->Read(buffer, offset, count);
	}
	Int LzxDecoderStream::ReadByte()
	{
		return Int();
	}
	void LzxDecoderStream::Write(Byte const* buffer, Int bufferLength, Int offset, Int count)
	{
	}
	void LzxDecoderStream::Write(std::vector<Byte> const& buffer, Int offset, Int count)
	{
	}
	void LzxDecoderStream::WriteByte(Byte value)
	{
	}
}