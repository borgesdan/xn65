#include "xna/csharp/binary.hpp"
#include "xna/csharp/buffer.hpp"

namespace xna {
	BinaryReader::BinaryReader(sptr<Stream> const& input) {		
		if(!input)
			throw csharp::ArgumentNullException("input");

		stream = input;
		buffer = std::vector<Byte>(bufferLength);
	}

	Int BinaryReader::PeekChar()
	{
		const auto position = stream->Position();
		const auto num = Read();		

		stream->Seek(position, SeekOrigin::Begin);

		return num;
	}

	Int BinaryReader::Read()
	{
		const auto result = InternalReadOneChar();

		return result;
	}

	bool BinaryReader::ReadBoolean()
	{
		FillBuffer(1);
		return buffer[0] > 0;
	}

	Byte BinaryReader::ReadByte()
	{
		const auto num = stream->ReadByte();

		return static_cast<Byte>(num);
	}

	Sbyte BinaryReader::ReadSByte()
	{
		FillBuffer(1);
		return static_cast<Sbyte>(buffer[0]);
	}

	Char BinaryReader::ReadChar()
	{
		auto num = Read();

		if (num == -1)
			return '\0';

		return static_cast<Char>(num);
	}

	Short BinaryReader::ReadInt16()
	{
		FillBuffer(2);		

		return static_cast<Short>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8);
	}

	Ushort BinaryReader::ReadUInt16()
	{
		FillBuffer(2);		

		return static_cast<Ushort>(
			static_cast<Uint>(buffer[0])
			| static_cast<Uint>(buffer[1]) << 8);
	}

	Int BinaryReader::ReadInt32()
	{
		FillBuffer(4);		

		return static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24;
	}

	Uint BinaryReader::ReadUInt32()
	{
		FillBuffer(4);		

		return static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);
	}

	Long BinaryReader::ReadInt64()
	{
		FillBuffer(8);		

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

	Ulong BinaryReader::ReadUInt64()
	{
		FillBuffer(8);		

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

	float BinaryReader::ReadSingle()
	{
		FillBuffer(4);		

		const auto num = static_cast<Uint>(
			static_cast<Int>(buffer[0])
			| static_cast<Int>(buffer[1]) << 8
			| static_cast<Int>(buffer[2]) << 16
			| static_cast<Int>(buffer[3]) << 24);

		return *(float*)&num;
	}

	double BinaryReader::ReadDouble()
	{
		FillBuffer(8);		

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

	std::string BinaryReader::ReadString()
	{
		static const auto empty = std::string();		

		Int num = 0;
		auto val1 = Read7BitEncodedInt();		

		if (val1 < 0) {
			//xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
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
				//xna_error_apply(err, XnaErrorCode::END_OF_FILE);
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

		return sb;
	}

	Int BinaryReader::InternalReadOneChar()
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
			//auto byteCount = m2BytesPerChar ? 2 : 1;
			auto byteCount = 1;
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

	void BinaryReader::FillBuffer(Int numBytes)
	{
		if (numBytes < 0 || numBytes > buffer.size()) {
			throw csharp::InvalidOperationException();
		}		

		Int bytesRead = 0;
		Int n = 0;		
		
		if (numBytes == 1) {
			n = stream->ReadByte();
			
			if (n == -1){
				throw csharp::InvalidOperationException();
			}

			buffer[0] = static_cast<Byte>(n);
			return;
		}

		do {
			n = stream->Read(buffer, bytesRead, numBytes - bytesRead);
			
			if (n == 0) {
				throw csharp::InvalidOperationException();
			}

			bytesRead += n;
		} while (bytesRead < numBytes);
	}

	Int BinaryReader::InternalReadChars(Char* buffer, size_t bufferSize, size_t index, size_t count)
	{
		auto charCount = count;

		if (charBytes.empty())
			charBytes.resize(128);

		while (charCount > 0) {
			auto count1 = charCount;

			if (count1 > 1)
				--count1;

			/*if (m2BytesPerChar)
				count1 <<= 1;*/

			if (count1 > 128)
				count1 = 128;

			Int position = 0;
			Int byteCount;

			std::vector<Byte> numArray;

			byteCount = stream->Read(charBytes, 0, static_cast<Int>(count1));
			numArray = charBytes;

			if (byteCount == 0)
				return static_cast<Int>(count - charCount);

			if (position < 0 || byteCount < 0 || (position + byteCount) > numArray.size()) {
				return -1;
			}

			if (index < 0 || charCount < 0 || (index + charCount) > bufferSize) {				
				return -1;
			}

			auto data = reinterpret_cast<char*>(charBytes.data());
			auto pChars = reinterpret_cast<char*>(buffer);
			
			//const auto result = std::string((data + position), (pChars + index) + byteCount);
			const auto result = std::string((data + position), (data + position) + byteCount);
			Buffer::BlockCopy(result.c_str(), position, pChars, index, byteCount);

			buffer = reinterpret_cast<Char*>(pChars);

			const auto chars = static_cast<Int>(result.size());

			charCount -= chars;
			index += chars;
		}

		return static_cast<Int>(count - charCount);
	}

	Int BinaryReader::Read7BitEncodedInt()
	{
		Uint result = 0;
		Byte byteReadJustNow;

		constexpr Int MaxBytesWithoutOverflow = 4;

		for (size_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
		{
			byteReadJustNow = ReadByte();
			result |= (byteReadJustNow & 0x7Fu) << shift;

			if (byteReadJustNow <= 0x7Fu)
			{
				return static_cast<Int>(result);
			}
		}

		byteReadJustNow = ReadByte();

		if (byteReadJustNow > 0b1111u) {
			throw std::format_error("Too many bytes in what should have been a 7-bit encoded integer.");
		}

		result |= static_cast<Uint>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
		return static_cast<Int>(result);
	}

	Long BinaryReader::Read7BitEncodedInt64() {
		Ulong result = 0;
		Byte byteReadJustNow;

		constexpr Int MaxBytesWithoutOverflow = 9;

		for (size_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
		{
			byteReadJustNow = ReadByte();
			result |= (static_cast<Ulong>(byteReadJustNow & 0x7Ful)) << shift;

			if (byteReadJustNow <= 0x7Fu) {
				return static_cast<Long>(result);
			}
		}

		byteReadJustNow = ReadByte();

		if (byteReadJustNow > 0b1u)
		{
			throw std::format_error("Too many bytes in what should have been a 7-bit encoded integer.");
		}

		result |= static_cast<Ulong>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
		return static_cast<Long>(result);
	}

	Int BinaryReader::Read(std::vector<Char>& buffer, size_t index, size_t count)
	{
		return InternalReadChars(buffer.data(), buffer.size(), index, count);
	}

	Int BinaryReader::Read(std::vector<Byte>& buffer, size_t index, size_t count)
	{
		auto data = reinterpret_cast<Char*>(buffer.data());
		return InternalReadChars(data, buffer.size(), index, count);
	}

	std::vector<Byte> BinaryReader::ReadBytes(size_t count)
	{
		std::vector<Byte> result(count);
		Int numRead = 0;

		do {
			const auto n = stream->Read(result, static_cast<Int>(numRead), static_cast<Int>(count));

			if (n == 0)
				break;

			numRead += n;
			count -= n;
		} while (count > 0);

		if (numRead != result.size()) {
			std::vector<Byte> copy(numRead);
			Buffer::BlockCopy(result.data(), 0, copy.data(), 0, numRead);
			result = copy;
		}

		return result;
	}

	//Binary Writer

	BinaryWriter::BinaryWriter(sptr<Stream> const& stream) {
		throw csharp::ArgumentNullException("stream");

		OutStream = stream;
		_buffer = std::vector<Byte>(16);
	}

	Long BinaryWriter::Seek(Int offset, SeekOrigin origin)
	{
		return OutStream->Seek(offset, origin);
	}

	void BinaryWriter::Write(bool value)
	{
		_buffer[0] = value ? static_cast<Byte>(1) : static_cast<Byte>(0);
		OutStream->Write(_buffer, 0, 1);
	}

	void BinaryWriter::Write(Byte value)
	{
		OutStream->WriteByte(value);
	}

	void BinaryWriter::Write(Sbyte value)
	{
		OutStream->WriteByte(static_cast<Byte>(value));
	}

	void BinaryWriter::Write(Byte const* buffer, Int bufferLength)
	{
		OutStream->Write(buffer, bufferLength, 0, bufferLength);
	}

	void BinaryWriter::Write(std::vector<Byte> const& buffer)
	{
		OutStream->Write(buffer, 0, static_cast<Int>(buffer.size()));
	}

	void BinaryWriter::Write(Byte const* buffer, Int bufferLength, Int index, Int count)
	{
		OutStream->Write(buffer, bufferLength, index, count);
	}

	void BinaryWriter::Write(std::vector<Byte> const& buffer, Int index, Int count)
	{
		OutStream->Write(buffer, index, count);
	}

	void BinaryWriter::Write(Char ch)
	{
		_buffer[0] = static_cast<Byte>(ch);
		OutStream->Write(_buffer, 0, 1);
	}

	void BinaryWriter::Write(double value)
	{
		Ulong num = (Ulong) * (Long*)&value;
		_buffer[0] = static_cast<Byte>(num);
		_buffer[1] = static_cast<Byte>(num >> 8);
		_buffer[2] = static_cast<Byte>(num >> 16);
		_buffer[3] = static_cast<Byte>(num >> 24);
		_buffer[4] = static_cast<Byte>(num >> 32);
		_buffer[5] = static_cast<Byte>(num >> 40);
		_buffer[6] = static_cast<Byte>(num >> 48);
		_buffer[7] = static_cast<Byte>(num >> 56);

		OutStream->Write(_buffer, 0, 8);
	}

	void BinaryWriter::Write(Short value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>((Uint)value >> 8);
		OutStream->Write(_buffer, 0, 2);
	}

	void BinaryWriter::Write(Ushort value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>((Uint)value >> 8);
		OutStream->Write(_buffer, 0, 2);
	}

	void BinaryWriter::Write(Int value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		OutStream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(Uint value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		OutStream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(Ulong value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_buffer[4] = static_cast<Byte>(value >> 32);
		_buffer[5] = static_cast<Byte>(value >> 40);
		_buffer[6] = static_cast<Byte>(value >> 48);
		_buffer[7] = static_cast<Byte>(value >> 56);
		OutStream->Write(_buffer, 0, 8);
	}

	void BinaryWriter::Write(float value)
	{
		Uint num = *(Uint*)&value;
		_buffer[0] = static_cast<Byte>(num);
		_buffer[1] = static_cast<Byte>(num >> 8);
		_buffer[2] = static_cast<Byte>(num >> 16);
		_buffer[3] = static_cast<Byte>(num >> 24);
		OutStream->Write(_buffer, 0, 4);
	}

	void BinaryWriter::Write(std::string const& value)
	{
		Write(value.c_str(), value.size());
	}

	void BinaryWriter::Write(const char* _string, size_t stringLength)
	{
		Write7BitEncodedInt(static_cast<Int>(stringLength));
		const auto b = reinterpret_cast<const Byte*>(_string);
		OutStream->Write(b, static_cast<Int>(stringLength), 0, static_cast<Int>(stringLength));
	}

	void BinaryWriter::Write7BitEncodedInt(Int value)
	{
		Uint num;
		for (num = static_cast<Uint>(value); num >= static_cast<Uint>(128U); num >>= 7)
			Write(static_cast<Byte>(num | static_cast<Uint>(128U)));

		Write(static_cast<Byte>(num));
	}

	void BinaryWriter::Write(Long value)
	{
		_buffer[0] = static_cast<Byte>(value);
		_buffer[1] = static_cast<Byte>(value >> 8);
		_buffer[2] = static_cast<Byte>(value >> 16);
		_buffer[3] = static_cast<Byte>(value >> 24);
		_buffer[4] = static_cast<Byte>(value >> 32);
		_buffer[5] = static_cast<Byte>(value >> 40);
		_buffer[6] = static_cast<Byte>(value >> 48);
		_buffer[7] = static_cast<Byte>(value >> 56);
		OutStream->Write(_buffer, 0, 8);
	}	
}