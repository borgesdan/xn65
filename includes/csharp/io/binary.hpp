#ifndef CSHARP_IO_BINARY_CPP
#define CSHARP_IO_BINARY_CPP

#include "stream.hpp"
#include "exception.hpp"
#include <optional>
#include <cstdint>

namespace csharp {
	/*
	* The BinaryReader class uses byte encodings, by default UTF8.
	* This was not implemented, but we tried to follow the same standard.
	* Also the reading of primitives was modified.
	*
	*/

	//TODO: ReadString and ReadChar as it only reads ASCII characters
	//https://learn.microsoft.com/pt-br/dotnet/csharp/language-reference/builtin-types/char
	//char - 16 bits

	//The BinaryReader class uses byte encodings, by default UTF8
	class BinaryReader {
	public:
		BinaryReader(std::shared_ptr<Stream> const& input, bool leaveOpen = false)
			: _stream(input), _leaveOpen(leaveOpen)
		{
			ArgumentNullException::ThrowIfNull(input.get(), "input");

			if (!input->CanRead())
				throw ArgumentException(SR::Argument_StreamNotReadable);
		}

		virtual std::shared_ptr<Stream> BaseStream() const { return _stream; }

		virtual void Close() {
			if (_disposed)
				return;

			if (!_leaveOpen)
				_stream->Close();

			_disposed = true;
		}

		virtual int32_t PeekChar();
		virtual int32_t Read(bool twoBytesPerChar = false);

		virtual uint8_t ReadByte() {
			return InternalReadByte();
		}

		virtual inline int8_t ReadSByte() {
			return static_cast<int8_t>(InternalReadByte());
		}

		virtual inline bool ReadBoolean() {
			return InternalReadByte() != 0;
		}

		virtual char ReadChar(bool twoBytesPerChar = false);

		virtual int16_t ReadInt16() {
			return ReadNumeric<int16_t>();
		}

		virtual uint16_t ReadUInt16() {
			return ReadNumeric<uint16_t>();
		}

		virtual int32_t ReadInt32() {
			return ReadNumeric<int32_t>();
		}

		virtual uint32_t ReadUInt32() {
			return ReadNumeric<uint32_t>();
		}

		virtual int64_t ReadInt64() {
			return ReadNumeric<int64_t>();
		}

		virtual uint64_t ReadUInt64() {
			return ReadNumeric<uint64_t>();
		}

		virtual float ReadSingle() {
			return ReadNumeric<float>();
		}

		virtual double ReadDouble() {
			return ReadNumeric<double>();
		}

		virtual std::string ReadString();
		virtual std::u8string ReadString8();

		virtual int32_t Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count);

		virtual int32_t Read(char* buffer, int32_t bufferLength);

		virtual std::vector<char> ReadChars(int32_t count);

		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count);

		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);

		virtual std::vector<uint8_t> ReadBytes(int32_t count);

		virtual void ReadExactly(uint8_t* buffer, int32_t bufferLength);

		int32_t Read7BitEncodedInt();
		int64_t Read7BitEncodedInt64();

		template <class TSTRING>
		TSTRING GenericReadString() {
			if (_disposed)
				throw InvalidOperationException();

			const auto stringLength = Read7BitEncodedInt();
			if (stringLength < 0)
			{
				throw IOException(SR::IO_InvalidStringLen_Len);
			}

			if (stringLength == 0)
			{
				return {};
			}

			auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);
			int32_t currPos = 0;

			TSTRING sb{};

			do
			{
				const auto readLength = std::min(MaxCharBytesSize, stringLength - currPos);
				const auto n = _stream->Read(charBytes.data(), readLength);

				if (n == 0)
				{
					throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
				}

				const auto chars = reinterpret_cast<TSTRING::value_type*>(charBytes.data());

				if (currPos == 0 && n == stringLength)
				{
					return TSTRING(chars);
				}

				sb.append(chars);

				currPos += n;

			} while (currPos < stringLength);

			return sb;
		}

	private:
		uint8_t InternalReadByte();
		void InternalRead(std::vector<uint8_t>& buffer);
		int32_t InternalReadChars(char* buffer, int32_t bufferLength);

		template<class TNUMERIC>
		TNUMERIC ReadNumeric() {
			const auto numericSize = sizeof(TNUMERIC);

			if (_auxBuffer.size() != numericSize)
				_auxBuffer.resize(numericSize);

			InternalRead(_auxBuffer);

			const auto ptr = reinterpret_cast<TNUMERIC*>(_auxBuffer.data());
			const auto value = *ptr;
			return value;
		}

	private:
		static constexpr int MaxCharBytesSize = 128;

		std::shared_ptr<Stream> _stream;
		bool _leaveOpen;
		bool _disposed{ false };

		std::vector<uint8_t> _auxBuffer;
	};

	class BinaryWriter {
	public:
		BinaryWriter(std::shared_ptr<Stream> const& output)
			: BinaryWriter(output, false) {
		}

		BinaryWriter(std::shared_ptr<Stream> const& output, bool leaveOpen) {
			ArgumentNullException::ThrowIfNull(output.get(), "output");

			if (!output->CanWrite())
				throw ArgumentException(SR::Argument_StreamNotWritable);

			OutStream = output;
			_leaveOpen = leaveOpen;
		}

		inline void Close() const {
			if (_leaveOpen)
				OutStream->Flush();
			else
				OutStream->Close();
		}

		inline virtual std::shared_ptr<Stream> BaseStream() {
			Flush();
			return OutStream;
		}

		inline virtual void Flush() {
			OutStream->Flush();
		}

		inline virtual int64_t Seek(int32_t offset, SeekOrigin origin) {
			return OutStream->Seek(offset, origin);
		}

		inline virtual void Write(bool value) {
			OutStream->WriteByte(static_cast<uint8_t>(value ? 1 : 0));
		}

		inline virtual void Write(uint8_t value) {
			OutStream->WriteByte(value);
		}

		inline virtual void Write(int8_t value) {
			OutStream->WriteByte(static_cast<uint8_t>(value));
		}

		inline virtual void Write(uint8_t const* buffer, int32_t bufferLength) {
			OutStream->Write(buffer, bufferLength, 0, bufferLength);
		}

		inline virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t index, int32_t count) {
			OutStream->Write(buffer, bufferLength, index, count);
		}

		inline virtual void Write(char ch) {
			const auto byte = static_cast<uint8_t>(ch);
			OutStream->Write(&byte, 1);
		}

		inline virtual void Write(char* chars, int32_t charsLength) {
			ArgumentNullException::ThrowIfNull(chars, "chars");
			auto bytes = reinterpret_cast<const uint8_t*>(chars);
			OutStream->Write(bytes, charsLength);
		}

		inline virtual void Write(char* chars, int32_t charsLength, int32_t index, int32_t count) {
			ArgumentNullException::ThrowIfNull(chars, "chars");
			ArgumentOutOfRangeException::ThrowIfNegative(index, "index");
			ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

			if (index > charsLength - count)
				throw ArgumentOutOfRangeException("index");

			auto bytes = reinterpret_cast<const uint8_t*>(chars);
			OutStream->Write(bytes, charsLength, index, count);
		}

		inline virtual void Write(double value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int16_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint16_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int32_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint32_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int64_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint64_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(float value) {
			WriteNumeric(value);
		}

		virtual void Write(std::string const& value) {
			Write7BitEncodedInt(static_cast<int64_t>(value.size()));
			auto bytes = reinterpret_cast<const uint8_t*>(value.data());
			OutStream->Write(bytes, value.size());
		}		

		void Write7BitEncodedInt(int32_t value);
		void Write7BitEncodedInt(int64_t value);

	protected:
		BinaryWriter() {
			OutStream = Stream::Null;
		}

		template <typename TNUMERIC>
		void WriteNumeric(TNUMERIC const& value) {
			auto bytes = reinterpret_cast<const uint8_t*>(&value);
			const auto size = sizeof(value);
			OutStream->Write(bytes, static_cast<int32_t>(size));
		}

	protected:
		std::shared_ptr<Stream> OutStream;

	private:
		static constexpr int MaxArrayPoolRentalSize = 64 * 1024;
		static std::shared_ptr<BinaryWriter> Null;
		bool _leaveOpen{ false };
		bool _useFastUtf8{ true };
	};
}

#endif