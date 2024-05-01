#include "decstream.hpp"
#include <iostream>

namespace xna {
	void DecompressStream::Decompress()
	{
		String filename = "C:\\Users\\Danilo Borges\\Documents\\xna\\projeto-final\\Game9\\bin\\Debug\\netcoreapp3.1\\Content\\Sprites\\MonsterA\\idle.xnb";
		
		
	}

	Int DecompressStream::Length()
	{
		return Int();
	}

	Long DecompressStream::Position()
	{
		return Long();
	}

	void DecompressStream::Close()
	{
	}

	Long DecompressStream::Seek(Long offset, SeekOrigin const& origin, xna_error_ptr_arg)
	{
		return Long();
	}

	Int DecompressStream::Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg)
	{
		return Int();
	}

	Int DecompressStream::Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_ptr_arg)
	{
		return Int();
	}

	Int DecompressStream::ReadByte(xna_error_ptr_arg)
	{
		return Int();
	}

	void DecompressStream::Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg)
	{
	}

	void DecompressStream::Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_ptr_arg)
	{
	}

	void DecompressStream::WriteByte(Byte value, xna_error_ptr_arg)
	{
	}

	bool DecompressStream::DecompressNextBuffer()
	{
		if (decompressedTodo <= 0)
			return false;

		do {
			if (compressedPosition >= compressedSize)
				ReadNextBufferFromDisk();

			auto sourceSize = compressedSize - compressedPosition;
			auto outputSize = 65536;

			//TODO

			if (outputSize == 0 && sourceSize == 0)
				return false;

			compressedPosition += sourceSize;
			decompressedTodo -= outputSize;
			decompressedSize = outputSize;
			decompressedPosition = 0;
			
		} while (decompressedSize == 0);

		return true;
	}

	void DecompressStream::ReadNextBufferFromDisk()
	{
		if (compressedTodo <= 0)
			return;

		ReadBufferFromDisk(compressedBuffer, compressedTodo, compressedSize);
		compressedPosition = 0;
	}

	void DecompressStream::ReadBufferFromDisk(std::vector<Byte>& buffer, Int& bufferTodo, Int& bufferSize)
	{
		Int num1 = 65536;
		if (num1 > bufferTodo)
			num1 = bufferTodo;

		Int num2 = 0;
		for (int offset = 0; offset < num1; offset += num2) {
			num2 = baseStream->Read(buffer, offset, num1 - offset);

			if (num2 == 0) return;
		}

		bufferTodo -= num1;
		bufferSize = num1;
	}

}