#include "binary.hpp"

namespace xna {
	Int BinaryReader::PeekChar(xna_error_ptr_arg)
	{
		if (!stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return -1;
		}		

		const auto position = stream->Position();
		const auto num = Read(err);

		if (xna_error_haserros(err))
			return -1;

		stream->Seek(position, SeekOrigin::Begin, err);

		if (xna_error_haserros(err))
			return -1;

		return num;
	}

	Int BinaryReader::Read(xna_error_ptr_arg)
	{
		if (!stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return -1;
		}

		const auto result = InternalReadOneChar(err);

		return xna_error_haserros(err) ? -1 : result;
	}

	bool BinaryReader::ReadBoolean(xna_error_ptr_arg)
	{
		FillBuffer(1, err);
		return xna_error_haserros(err) ? false : buffer[0] > 0;
	}

	Byte BinaryReader::ReadByte(xna_error_ptr_arg)
	{
		if (!stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return -1;
		}

		const auto num = stream->ReadByte();

		if (num == -1)
		{
			xna_error_apply(err, XnaErrorCode::END_OF_FILE);
			return 0;
		}

		return static_cast<Byte>(num);
	}

	Sbyte BinaryReader::ReadSByte(xna_error_ptr_arg)
	{
		FillBuffer(1, err);
		return xna_error_haserros(err) ? -1 : static_cast<Sbyte>(buffer[0]);
	}

	Char BinaryReader::ReadChar(xna_error_ptr_arg)
	{
		auto num = Read(err);

		if (xna_error_haserros(err) || num == -1)
			return '\0';

		return static_cast<Char>(num);
	}

	Short BinaryReader::ReadInt16(xna_error_ptr_arg)
	{
		FillBuffer(2, err);

		if (xna_error_haserros(err))
			return -1;

		return static_cast<Short>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8);
	}

	Ushort BinaryReader::ReadUInt16(xna_error_ptr_arg)
	{
		FillBuffer(2, err);

		if (xna_error_haserros(err))
			return 0;

		return static_cast<Ushort>(
			static_cast<Uint>(buffer[0])
			| static_cast<Uint>(buffer[1]) << 8);
	}

	Int BinaryReader::ReadInt32(xna_error_ptr_arg)
	{
		FillBuffer(4, err);

		if (xna_error_haserros(err))
			return -1;

		return static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24;
	}

	Uint BinaryReader::ReadUInt32(xna_error_ptr_arg)
	{
		FillBuffer(4, err);

		if (xna_error_haserros(err))
			return -1;

		return static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);
	}

	Long BinaryReader::ReadInt64(xna_error_ptr_arg)
	{
		FillBuffer(8, err);

		if (xna_error_haserros(err))
			return -1;

		const auto num1 = static_cast<Uint>(
			static_cast<Int>(buffer[4])
			| static_cast<Int>(buffer[5]) << 8
			| static_cast<Int>(buffer[6]) << 16
			| static_cast<Int>(buffer[7]) << 24);

		const auto num2 = static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);

		return static_cast<Long>(num1) << 32 | static_cast<Long>(num2);
	}

	Ulong BinaryReader::ReadUInt64(xna_error_ptr_arg)
	{
		FillBuffer(8, err);

		if (xna_error_haserros(err))
			return 0;

		const auto num1 = static_cast<Uint>(
			static_cast<Int>(buffer[4])
			| static_cast<Int>(buffer[5]) << 8
			| static_cast<Int>(buffer[6]) << 16
			| static_cast<Int>(buffer[7]) << 24);

		const auto num2 = static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);

		return static_cast<Ulong>(num1) << 32 | static_cast<Ulong>(num2);
	}

	float BinaryReader::ReadSingle(xna_error_ptr_arg)
	{
		FillBuffer(4, err);

		if (xna_error_haserros(err))
			return std::numeric_limits<float>::quiet_NaN();

		const auto num = static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);

		return *(float*)&num;
	}

	double BinaryReader::ReadDouble(xna_error_ptr_arg)
	{
		FillBuffer(8, err);

		if (xna_error_haserros(err))
			return std::numeric_limits<double>::quiet_NaN();

		const auto num1 = static_cast<Uint>(
			static_cast<Int>(buffer[4])
			| static_cast<Int>(buffer[5]) << 8
			| static_cast<Int>(buffer[6]) << 16
			| static_cast<Int>(buffer[7]) << 24);

		const auto num2 = static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 6
			| static_cast<Int>(buffer[3]) << 24);

		const auto num3 = static_cast<Ulong>(num1) << 32 | static_cast<Ulong>(num2);

		return *(double*)&num3;
	}

	std::string BinaryReader::ReadString(xna_error_ptr_arg)
	{
		static const auto empty = std::string();

		if (!stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return std::string();
		}

		Int num = 0;
		auto val1 = Read7BitEncodedInt(err);

		if (xna_error_haserros(err))
			return empty;

		if (val1 < 0) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return empty;
		}

		if (val1 == 0)
			return empty;

		if (charBytes.empty())
			charBytes.resize(maxCharBytesSize);

		if (charBuffer.empty())
			charBuffer.resize(maxCharBytesSize);

		std::string sb;

		do {
			const auto byteCount = stream->Read(charBytes, 0, val1 - num > 128 ? 128 : val1 - num);

			if (byteCount == 0) {
				xna_error_apply(err, XnaErrorCode::END_OF_FILE);
				return empty;
			}

			auto data = reinterpret_cast<char*>(charBytes.data());
			const auto result = std::string(data);

			if (num == 0 && byteCount == val1) {
				return result;
			}

			sb.append(result);
			num += byteCount;

		} while (num < val1);

		return empty;
	}

	Int BinaryReader::InternalReadOneChar(xna_error_ptr_arg)
	{
		Int num1 = 0;
		Long num2 = 0;
		Long num3 = stream->Position();		

		if (charBytes.empty())
			charBytes.resize(128);

		if (singleChar.empty())
			singleChar.resize(1);

		while (num1 == 0)
		{
			auto byteCount = m2BytesPerChar ? 2 : 1;
			const auto num4 = stream->ReadByte();

			charBytes[0] = static_cast<Byte>(num4);

			if (num4 == -1)
				byteCount = 0;

			if (byteCount == 2) {
				auto num5 = stream->ReadByte();
				charBytes[1] = static_cast<Byte>(num5);

				if (num5 == -1)
					byteCount = 1;
			}

			if (byteCount == 0) {
				return -1;
			}

			auto data = reinterpret_cast<char*>(charBytes.data());
			const auto result = std::string(data, data + byteCount);

			if (!result.empty())
			{
				num1 = static_cast<Int>(result.size());
				singleChar[0] = result[0];
			}
		}

		return num1 == 0 ? -1 : static_cast<Int>(singleChar[0]);
	}

	void BinaryReader::FillBuffer(Int numBytes, xna_error_ptr_arg)
	{
		if (!stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		if (!buffer.empty() && (numBytes < 0 || numBytes > buffer.size())) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
			return;
		}

		int offset = 0;

		if (numBytes == 1)
		{

			const auto num = stream->ReadByte();
			if (num == -1) {
				xna_error_apply(err, XnaErrorCode::END_OF_FILE);
				return;
			}

			buffer[0] = static_cast<Byte>(num);
		}
		else
		{
			do
			{
				const auto num = stream->Read(buffer, offset, numBytes - offset);

				if (num == 0)
				{
					xna_error_apply(err, XnaErrorCode::END_OF_FILE);
					return;
				}

				offset += num;
			} while (offset < numBytes);
		}
	}

	Int BinaryReader::InternalReadChars(char* buffer, size_t bufferSize, Int index, Int count, xna_error_ptr_arg)
	{
		auto charCount = count;

		if (charBytes.empty())
			charBytes.resize(128);

		while (charCount > 0) {
			auto count1 = charCount;

			if (count1 > 1)
				--count1;

			if (m2BytesPerChar)
				count1 <<= 1;

			if (count1 > 128)
				count1 = 128;

			Int num = 0;
			Int byteCount;

			std::vector<Byte> numArray;

			byteCount = stream->Read(charBytes, 0, count1);
			numArray = charBytes;

			if (byteCount == 0)
				return count - charCount;

			if (num < 0 || byteCount < 0 || (num + byteCount) > numArray.size()) {
				xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
				return -1;
			}

			if (index < 0 || charCount < 0 || (index + charCount) > bufferSize) {
				xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
				return -1;
			}

			auto data = reinterpret_cast<char*>(charBytes.data());
			const auto result = std::string((data + num), (data + num) + byteCount);

			const auto chars = result.size();

			charCount -= static_cast<Int>(chars);
			index += static_cast<Int>(chars);
		}

		return count - charCount;
	}

	Long BinaryWriter::Seek(Int offset, SeekOrigin origin, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return - 1;
		}

		return _stream->Seek(offset, origin);
	}

	void BinaryWriter::Write(bool value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = value ? (Byte)1 : (Byte)0;
		_stream->Write(_buffer, 0, 1);
	}

	void BinaryWriter::Write(Byte value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->WriteByte(value);
	}

	void BinaryWriter::Write(Sbyte value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->WriteByte(static_cast<Byte>(value));
	}

	void BinaryWriter::Write(Byte const* buffer, Int bufferLength, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->Write(buffer, bufferLength, 0, bufferLength);
	}

	void BinaryWriter::Write(std::vector<Byte> const& buffer, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->Write(buffer, 0, static_cast<Int>(buffer.size()));
	}
	void BinaryWriter::Write(Byte const* buffer, Int bufferLength, Int index, Int count, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->Write(buffer, bufferLength, index, count);
	}

	void BinaryWriter::Write(std::vector<Byte> const& buffer, Int index, Int count, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_stream->Write(buffer, index, count);
	}

	void BinaryWriter::Write(Char ch, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(ch);
		_stream->Write(_buffer, 0, 1);
	}

	void BinaryWriter::Write(double value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		Ulong num = (Ulong) * (Long*)&value;
		_buffer[0] = static_cast<Byte>(num);
		_buffer[1] = static_cast<Byte>(num >> 8);
		_buffer[2] = static_cast<Byte>(num >> 16);
		_buffer[3] = static_cast<Byte>(num >> 24);
		_buffer[4] = static_cast<Byte>(num >> 32);
		_buffer[5] = static_cast<Byte>(num >> 40);
		_buffer[6] = static_cast<Byte>(num >> 48);
		_buffer[7] = static_cast<Byte>(num >> 56);

		_stream->Write(_buffer, 0, 8);
	}

	void BinaryWriter::Write(Short value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>((Uint)value >> 8);
		_stream->Write(_buffer, 0, 2);
	}

	void BinaryWriter::Write(Ushort value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>((Uint)value >> 8);
		_stream->Write(_buffer, 0, 2);
	}

	void BinaryWriter::Write(Int value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_stream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(Uint value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_stream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(Ulong value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_buffer[4] = static_cast<Byte>(value >> 32);
		_buffer[5] = static_cast<Byte>(value >> 40);
		_buffer[6] = static_cast<Byte>(value >> 48);
		_buffer[7] = static_cast<Byte>(value >> 56);
		_stream->Write(_buffer, 0, 8);
	}

	void BinaryWriter::Write(float value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		Uint num = *(Uint*)&value;
		_buffer[0] = static_cast<Byte>(num);
		_buffer[1] = static_cast<Byte>(num >> 8);
		_buffer[2] = static_cast<Byte>(num >> 16);
		_buffer[3] = static_cast<Byte>(num >> 24);
		_stream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(std::string const& value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		Write(value.c_str(), value.size());
	}

	void BinaryWriter::Write(const char* _string, size_t stringLength, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		Write7BitEncodedInt(static_cast<Int>(stringLength));
		const auto b = reinterpret_cast<const Byte*>(_string);
		_stream->Write(b, static_cast<Int>(stringLength), 0, static_cast<Int>(stringLength));
	}

	void BinaryWriter::Write7BitEncodedInt(Int value)
	{
		Uint num;
		for (num = (Uint)value; num >= (Uint)128U; num >>= 7)
			Write((Byte)(num | (Uint)128U));

		Write((Byte)num);
	}

	void BinaryWriter::Write(Long value, xna_error_ptr_arg)
	{
		if (!_stream) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_buffer[4] = static_cast<Byte>(value >> 32);
		_buffer[5] = static_cast<Byte>(value >> 40);
		_buffer[6] = static_cast<Byte>(value >> 48);
		_buffer[7] = static_cast<Byte>(value >> 56);
		_stream->Write(_buffer, 0, 8);
	}
}