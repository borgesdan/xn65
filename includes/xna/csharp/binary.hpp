#ifndef XNA_CSHARP_BINARY_HPP
#define XNA_CSHARP_BINARY_HPP

#include "stream.hpp"
#include "../default.hpp"

namespace xna {
	//A simplified port of the System.IO.BinaryReader class.
	class BinaryReader {
	public:
		BinaryReader(sptr<Stream> const& input);

		//Returns the next available character and does not advance the byte or character position.
		Int PeekChar();
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
		Int Read7BitEncodedInt();	
		
		// Reads a 64-bit integer in compressed format.
		// This function may throw a std::format_error exception.
		Long Read7BitEncodedInt64();	

		//Exposes access to the underlying stream of the BinaryReader.
		virtual inline sptr<Stream> BaseStream() const {
			return stream;
		}

		//Closes the current reader and the underlying stream.
		virtual inline void Close() {
			stream = nullptr;
		}

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
	};	

	//A simplified port of the System.IO.BinaryWriter class.
	class BinaryWriter {
	public:
		BinaryWriter(sptr<Stream> const& stream);

	protected:
		BinaryWriter() = default;

		//Sets the position within the current stream.
		Long Seek(Int offset, SeekOrigin origin);

		//
		// Writes a value to the current stream.
		//

		//Writes a value to the current stream.
		virtual void Write(bool value);
		//Writes a value to the current stream.
		virtual void Write(Byte value);
		//Writes a value to the current stream.
		virtual void Write(Sbyte value);
		//Writes a value to the current stream.
		virtual void Write(Byte const* buffer, Int bufferLength);
		//Writes a value to the current stream.
		virtual void Write(std::vector<Byte> const& buffer);
		//Writes a value to the current stream.
		virtual void Write(Byte const* buffer, Int bufferLength, Int index, Int count);
		//Writes a value to the current stream.
		virtual void Write(std::vector<Byte> const& buffer, Int index, Int count);
		//Writes a value to the current stream.
		virtual void Write(Char ch);
		//Writes a value to the current stream.
		virtual void Write(double value);
		//Writes a value to the current stream.
		virtual void Write(Short value);
		//Writes a value to the current stream.
		virtual void Write(Ushort value);
		//Writes a value to the current stream.
		virtual void Write(Int value);
		//Writes a value to the current stream.
		virtual void Write(Uint value);
		//Writes a value to the current stream.
		virtual void Write(Long value);
		//Writes a value to the current stream.
		virtual void Write(Ulong value);
		//Writes a value to the current stream.
		virtual void Write(float value);
		//Writes a value to the current stream.
		virtual void Write(std::string const& value);
		//Writes a value to the current stream.
		virtual void Write(const char* _string, size_t stringLength);

		//Exposes access to the underlying stream of the BinaryWriter.
		virtual inline sptr<Stream> BaseStream() const {
			return OutStream;
		}		

		//Writes a 32-bit integer in a compressed format.
		void Write7BitEncodedInt(Int value);		
		
	protected:
		sptr<Stream> OutStream = nullptr;

	private:
		std::vector<Byte> _buffer;
	};
}

#endif