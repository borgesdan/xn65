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
	Long LzxDecoderStream::Seek(Long offset, SeekOrigin const& origin, xna_error_ptr_arg)
	{
		return Long();
	}
	Int LzxDecoderStream::Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg)
	{
		return decompressedStream->Read(buffer, bufferLength, offset, count, err);
	}
	Int LzxDecoderStream::Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_ptr_arg)
	{
		return decompressedStream->Read(buffer, offset, count, err);
	}
	Int LzxDecoderStream::ReadByte(xna_error_ptr_arg)
	{
		return Int();
	}
	void LzxDecoderStream::Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg)
	{
	}
	void LzxDecoderStream::Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_ptr_arg)
	{
	}
	void LzxDecoderStream::WriteByte(Byte value, xna_error_ptr_arg)
	{
	}
}