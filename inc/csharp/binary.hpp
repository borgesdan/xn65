#ifndef XNA_CSHARP_BINARY_HPP
#define XNA_CSHARP_BINARY_HPP

#include "stream.hpp"
#include "../default.hpp"

namespace xna {
	class BinaryReader {
	public:
		BinaryReader(sptr<Stream> const& input) {
			stream = input;
			buffer = std::vector<Byte>(bufferLength);
		}

		Int PeekChar(xna_error_nullarg);
		Int Read(xna_error_nullarg);		
		bool ReadBoolean(xna_error_nullarg);		
		Byte ReadByte(xna_error_nullarg);
		Sbyte ReadSByte(xna_error_nullarg);
		Char ReadChar(xna_error_nullarg);
		Short ReadInt16(xna_error_nullarg);
		Ushort ReadUInt16(xna_error_nullarg);
		Int ReadInt32(xna_error_nullarg);
		Uint ReadUInt32(xna_error_nullarg);
		Long ReadInt64(xna_error_nullarg);
		Ulong ReadUInt64(xna_error_nullarg);
		float ReadSingle(xna_error_nullarg);
		double ReadDouble(xna_error_nullarg);
		std::string ReadString(xna_error_nullarg);

		Int Read(std::vector<Char>& buffer, size_t index, size_t count, xna_error_nullarg);
		Int Read(std::vector<Byte>& buffer, size_t index, size_t count, xna_error_nullarg);

		std::vector<Byte> ReadBytes(size_t count, xna_error_nullarg);
		
		// Reads a 32-bit integer in compressed format.
		// This function may throw a std::format_error exception or returns -1 if stream is null.
		Int Read7BitEncodedInt() noexcept(false);		
		
		
		// Reads a 64-bit integer in compressed format.
		// This function may throw a std::format_error exception or returns -1 if stream is null.		
		Long Read7BitEncodedInt64() noexcept(false);
	private:
		static constexpr int maxCharBytesSize = 128;
		static constexpr int bufferLength = 16;
		sptr<Stream> stream = nullptr;
		std::vector<Byte> charBytes;
		std::vector<Char> singleChar;
		std::vector<Byte> buffer;
		std::vector<Char> charBuffer;

		bool m2BytesPerChar{ false };

	protected:
		Int InternalReadOneChar();		
		void FillBuffer(Int numBytes) noexcept(false);
		Int InternalReadChars(Char* buffer, size_t bufferSize, size_t index, size_t count, xna_error_nullarg);
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