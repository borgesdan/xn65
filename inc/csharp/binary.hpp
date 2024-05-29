#ifndef XNA_CSHARP_BINARY_HPP
#define XNA_CSHARP_BINARY_HPP

#include "stream.hpp"
#include "../default.hpp"

namespace xna {
	//A simplified port of the BinaryReader class.
	class BinaryReader {
	public:
		BinaryReader(sptr<Stream> const& input) {
			if (!input)
				throw std::invalid_argument("input is null.");

			stream = input;
			buffer = std::vector<Byte>(bufferLength);
		}

		//Returns the next available character and does not advance the byte or character position.
		Int PeekChar();
		//Reads bytes from the underlying stream and advances the current position of the stream.
		Int Read();		
		//Reads a Boolean value from the current stream and advances the current position of the stream by one byte.
		bool ReadBoolean();		
		//Reads the next byte from the current stream and advances the current position of the stream by one byte.
		Byte ReadByte();
		//Reads a signed byte from this stream and advances the current position of the stream by one byte.
		Sbyte ReadSByte();
		//Reads the next character from the current stream and advances the current position of the stream.
		Char ReadChar();
		//Reads a 2-byte signed integer from the current stream and advances the current position of the stream by two bytes.
		Short ReadInt16();
		//Reads a 2-byte unsigned integer from the current stream and advances the position of the stream by two bytes.
		Ushort ReadUInt16();
		//Reads a 4-byte signed integer from the current stream and advances the current position of the stream by four bytes.
		Int ReadInt32();
		//Reads a 4-byte unsigned integer from the current stream and advances the position of the stream by four bytes.
		Uint ReadUInt32();
		//Reads a 8-byte signed integer from the current stream and advances the current position of the stream by eight bytes.
		Long ReadInt64();
		//Reads a 8-byte unsigned integer from the current stream and advances the position of the stream by eight bytes.
		Ulong ReadUInt64();
		//Reads a 4-byte floating point value from the current stream and advances the current position of the stream by four bytes.
		float ReadSingle();
		//Reads an 8-byte floating point value from the current stream and advances the current position of the stream by eight bytes.
		double ReadDouble();
		//Reads a string from the current stream.
		std::string ReadString();

		//Reads chars from the underlying stream and advances the current position of the stream.
		Int Read(std::vector<Char>& buffer, size_t index, size_t count);
		//Reads bytes from the underlying stream and advances the current position of the stream.
		Int Read(std::vector<Byte>& buffer, size_t index, size_t count);

		// Reads the specified number of bytes from the current stream into a byte array
		// and advances the current position by that number of bytes.
		std::vector<Byte> ReadBytes(size_t count);
		
		// Reads a 32-bit integer in compressed format.
		// This function may throw a std::format_error exception.
		Int Read7BitEncodedInt();	
		
		// Reads a 64-bit integer in compressed format.
		// This function may throw a std::format_error exception.
		Long Read7BitEncodedInt64();	

	protected:
		Int InternalReadOneChar();
		void FillBuffer(Int numBytes);
		Int InternalReadChars(Char* buffer, size_t bufferSize, size_t index, size_t count);

	private:
		static constexpr int maxCharBytesSize = 128;
		static constexpr int bufferLength = 16;
		sptr<Stream> stream = nullptr;
		std::vector<Byte> charBytes;
		std::vector<Char> singleChar;
		std::vector<Byte> buffer;
		std::vector<Char> charBuffer;

		//bool m2BytesPerChar{ false };
	};

	class BinaryWriter {
	public:
		BinaryWriter(sptr<Stream> const& stream) : _stream(stream), _buffer(16) {
		}

		Long Seek(Int offset, SeekOrigin origin, xna_error_nullarg);

		void Write(bool value, xna_error_nullarg);
		void Write(Byte value, xna_error_nullarg);
		void Write(Sbyte value, xna_error_nullarg);
		void Write(Byte const* buffer, Int bufferLength, xna_error_nullarg);
		void Write(std::vector<Byte> const& buffer, xna_error_nullarg);
		void Write(Byte const* buffer, Int bufferLength, Int index, Int count, xna_error_nullarg);
		void Write(std::vector<Byte> const& buffer, Int index, Int count, xna_error_nullarg);
		void Write(Char ch, xna_error_nullarg);
		void Write(double value, xna_error_nullarg);
		void Write(Short value, xna_error_nullarg);
		void Write(Ushort value, xna_error_nullarg);
		void Write(Int value, xna_error_nullarg);
		void Write(Uint value, xna_error_nullarg);
		void Write(Long value, xna_error_nullarg);
		void Write(Ulong value, xna_error_nullarg);
		void Write(float value, xna_error_nullarg);
		void Write(std::string const& value, xna_error_nullarg);
		void Write(const char* _string, size_t stringLength, xna_error_nullarg);

	public:
		sptr<Stream> _stream = nullptr;

	private:
		std::vector<Byte> _buffer;
		void Write7BitEncodedInt(Int value);
	};
}

#endif