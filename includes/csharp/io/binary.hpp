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
		virtual int32_t Read();

		virtual uint8_t ReadByte() {
			return InternalReadByte();
		}
		
		virtual inline int8_t ReadSByte() {
			return static_cast<int8_t>(InternalReadByte());
		}

		virtual inline bool ReadBoolean() {
			return InternalReadByte() != 0;
		}

		virtual char ReadChar();
		
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

		virtual int32_t Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count);

		virtual int32_t Read(char* buffer, int32_t bufferLength);

		virtual std::vector<char> ReadChars(int32_t count);

		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count);

		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);

		virtual std::vector<uint8_t> ReadBytes(int32_t count);

		virtual void ReadExactly(uint8_t* buffer, int32_t bufferLength);

		int32_t Read7BitEncodedInt();
		int64_t Read7BitEncodedInt64();

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
		bool _disposed{false};
		bool _2BytesPerChar{ true };

		std::vector<uint8_t> _auxBuffer;		
	};

	class BinaryWriter {
	public:
		BinaryWriter(std::shared_ptr<Stream> const& output) {
			OutStream = Stream::Null;
		}

		BinaryWriter(std::shared_ptr<Stream> const& output, bool leaveOpen);

	protected:
		BinaryWriter();

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