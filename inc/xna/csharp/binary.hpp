#ifndef XNA_CSHARP_BINARY_HPP
#define XNA_CSHARP_BINARY_HPP

#include "stream.hpp"
#include "../default.hpp"

namespace xna {
	//A simplified port of the System.IO.BinaryReader class.
	class BinaryReader {
	public:
		BinaryReader(sptr<Stream> const& input) {
			if (!input)
				throw std::invalid_argument("input is null.");

			stream = input;
			buffer = std::vector<Byte>(bufferLength);
		}

		virtual ~BinaryReader() = default;

		//Returns the next available character and does not advance the byte or character position.
		virtual Int PeekChar();
		//Reads bytes from the underlying stream and advances the current position of the stream.
		virtual Int Read();
		//Reads a Boolean value from the current stream and advances the current position of the stream by one byte.
		virtual bool ReadBoolean();
		//Reads the next byte from the current stream and advances the current position of the stream by one byte.
		virtual Byte ReadByte();
		//Reads a signed byte from this stream and advances the current position of the stream by one byte.
		virtual Sbyte ReadSByte();
		//Reads the next character from the current stream and advances the current position of the stream.
		virtual Char ReadChar();
		//Reads a 2-byte signed integer from the current stream and advances the current position of the stream by two bytes.
		virtual Short ReadInt16();
		//Reads a 2-byte unsigned integer from the current stream and advances the position of the stream by two bytes.
		virtual Ushort ReadUInt16();
		//Reads a 4-byte signed integer from the current stream and advances the current position of the stream by four bytes.
		virtual Int ReadInt32();
		//Reads a 4-byte unsigned integer from the current stream and advances the position of the stream by four bytes.
		virtual Uint ReadUInt32();
		//Reads a 8-byte signed integer from the current stream and advances the current position of the stream by eight bytes.
		virtual Long ReadInt64();
		//Reads a 8-byte unsigned integer from the current stream and advances the position of the stream by eight bytes.
		virtual Ulong ReadUInt64();
		//Reads a 4-byte floating point value from the current stream and advances the current position of the stream by four bytes.
		virtual float ReadSingle();
		//Reads an 8-byte floating point value from the current stream and advances the current position of the stream by eight bytes.
		virtual double ReadDouble();
		//Reads a string from the current stream.
		virtual std::string ReadString();

		//Reads chars from the underlying stream and advances the current position of the stream.
		virtual Int Read(std::vector<Char>& buffer, size_t index, size_t count);
		//Reads bytes from the underlying stream and advances the current position of the stream.
		virtual Int Read(std::vector<Byte>& buffer, size_t index, size_t count);

		// Reads the specified number of bytes from the current stream into a byte array
		// and advances the current position by that number of bytes.
		virtual std::vector<Byte> ReadBytes(size_t count);
		
		// Reads a 32-bit integer in compressed format.
		// This function may throw a std::format_error exception.
		virtual Int Read7BitEncodedInt();
		
		// Reads a 64-bit integer in compressed format.
		// This function may throw a std::format_error exception.
		virtual Long Read7BitEncodedInt64();

	protected:
		Int InternalReadOneChar();
		void FillBuffer(Int numBytes);
		Int InternalReadChars(Char* buffer, size_t bufferSize, size_t index, size_t count);

	public:
		sptr<Stream> stream = nullptr;

	private:
		static constexpr int maxCharBytesSize = 128;
		static constexpr int bufferLength = 16;
		std::vector<Byte> charBytes;
		std::vector<Char> singleChar;
		std::vector<Byte> buffer;
		std::vector<Char> charBuffer;

		//bool m2BytesPerChar{ false };
	};

	//A simplified port of the System.IO.BinaryWriter class.
	class BinaryWriter {
	public:
		BinaryWriter(){}

		BinaryWriter(sptr<Stream> const& stream) {
			if (!stream)
				throw std::invalid_argument("stream is null.");

			_stream = stream;
			_buffer = std::vector<Byte>(16);
		}

		virtual ~BinaryWriter() = default;

		//Sets the position within the current stream.
		virtual Long Seek(Int offset, SeekOrigin origin);

		//
		// Writes a value to the current stream.
		//

		virtual void Write(bool value);
		virtual void Write(Byte value);
		virtual void Write(Sbyte value);
		virtual void Write(Byte const* buffer, Int bufferLength);
		virtual void Write(std::vector<Byte> const& buffer);
		virtual void Write(Byte const* buffer, Int bufferLength, Int index, Int count);
		virtual void Write(std::vector<Byte> const& buffer, Int index, Int count);
		virtual void Write(Char ch);
		virtual void Write(double value);
		virtual void Write(Short value);
		virtual void Write(Ushort value);
		virtual void Write(Int value);
		virtual void Write(Uint value);
		virtual void Write(Long value);
		virtual void Write(Ulong value);
		virtual void Write(float value);
		virtual void Write(std::string const& value);
		virtual void Write(const char* _string, size_t stringLength);

		//Writes a 32-bit integer in a compressed format.
		void Write7BitEncodedInt(Int value);
		
		//void Write7BitEncodedInt64(Long value);	

	public:
		sptr<Stream> _stream = nullptr;
	private:
		std::vector<Byte> _buffer;
	};
}

#endif