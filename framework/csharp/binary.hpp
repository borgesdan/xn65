#ifndef XNA_CSHARP_BINARY_HPP
#define XNA_CSHARP_BINARY_HPP

#include "stream.hpp"
#include "../default.hpp"

namespace xna {
	class BinaryReader {
	public:
		BinaryReader(Stream* const& input) {
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

		Int Read(std::vector<Char>& buffer, size_t index, size_t count, xna_error_nullarg) {
			return -1;
		}

		std::vector<Byte> ReadBytes(size_t count, xna_error_nullarg) {
			return std::vector<Byte>();
		}

	private:
		static constexpr int maxCharBytesSize = 128;
		static constexpr int bufferLength = 16;
		Stream* stream = nullptr;
		std::vector<Byte> charBytes;
		std::vector<Char> singleChar;
		std::vector<Byte> buffer;
		std::vector<Char> charBuffer;

		bool m2BytesPerChar{ false };

		Int InternalReadOneChar(xna_error_nullarg);

		void FillBuffer(Int numBytes, xna_error_nullarg);

		Int Read7BitEncodedInt(xna_error_nullarg)
		{
			Int num1 = 0;
			Int num2 = 0;

			while (num2 != 35) {
				auto num3 = ReadByte(err);

				if (xna_error_haserros(err))
					return -1;

				num1 |= (static_cast<Int>(num3) & static_cast<Int>(SbyteMaxValue)) << num2;
				num2 += 7;

				if ((static_cast<Int>(num3) & 128) == 0)
					return num1;
			}

			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return -1;
		}

		Int InternalReadChars(char* buffer, size_t bufferSize, Int index, Int count, xna_error_nullarg);
	};

	class BinaryWriter {
	public:
		BinaryWriter(Stream* stream) : _stream(stream), _buffer(16) {
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
		Stream* _stream;

	private:
		std::vector<Byte> _buffer;
		void Write7BitEncodedInt(Int value);
	};
}

#endif