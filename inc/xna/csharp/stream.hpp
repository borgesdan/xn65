#ifndef XNA_CSHARP_STREAM_HPP
#define XNA_CSHARP_STREAM_HPP

#include "../types.hpp"
#include "../enums.hpp"
#include <fstream>
#include <filesystem>

namespace xna {
	//A simplified port of the System.IO.Stream.
	//Provides a generic view of a sequence of bytes. This is an abstract class.
	class Stream {
	public:
		virtual ~Stream(){}
		//Gets the length in bytes of the stream.
		virtual Int Length() = 0;
		//Gets the position within the current stream.
		virtual Long Position() = 0;
		//Closes the current stream and releases any resources
		virtual void Close() = 0;
		virtual bool IsClosed() = 0;
		//Sets the position within the current stream.
		virtual Long Seek(Long offset, SeekOrigin const& origin) = 0;
		
		//
		//Reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.
		//		
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count) = 0;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count) = 0;
		
		//Reads a byte from the stream and advances the position within the stream by one byte, or returns -1 if at the end of the stream.
		virtual Int ReadByte() = 0;
		
		//
		//When overridden in a derived class, writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
		//
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count) = 0;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count) = 0;
		
		//Writes a byte to the current position in the stream and advances the position within the stream by one byte.
		virtual void WriteByte(Byte value) = 0;

		virtual void* Data() = 0;
	};

	//A simplified port of the System.IO.MemoryStream.
	class MemoryStream : public Stream {
	public:
		constexpr MemoryStream(std::vector<Byte> const& bytes):
			_buffer(bytes), _length(bytes.size()){}

		constexpr MemoryStream(Int capacity) :
			_buffer(static_cast<size_t>(capacity)),
			_length(capacity > 0 ? capacity : 0){}

		virtual constexpr Int Length() override {
			if (_closed)
				return 0;

			return _length;
		}

		virtual constexpr Long Position() override {
			if (_closed)
				return 0;

			return _position;
		}

		virtual constexpr void Close() override {
			_closed = true;
			_buffer = std::vector<Byte>();
		}

		virtual constexpr bool IsClosed() override {
			return _closed;
		}

		virtual Long Seek(Long offset, SeekOrigin const& origin) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count) override;
		virtual Int ReadByte() override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count) override;
		virtual void WriteByte(Byte value) override;
	
		virtual void* Data() override {
			return _buffer.data();
		}

	public:
		std::vector<Byte> _buffer;
	private:
		Int _position{ 0 };
		Int _origin{ 0 };
		Int _length{ 0 };
		bool _closed{ false };
	};

	//A simplified port of the System.IO.FileStream.
	class FileStream : public Stream {
	public:
		FileStream(String const& path, FileMode fileMode);
		FileStream(String const& path);

		~FileStream() {
			Close();
		}		

		virtual Int Length() override;
		virtual Long Position() override;

		inline virtual void Close() override {
			_closed = true;

			if(_fstream.is_open())
				_fstream.close();
		}

		inline virtual constexpr bool IsClosed() override {
			return _closed;
		}

		virtual Long Seek(Long offset, SeekOrigin const& origin) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count) override;
		virtual Int ReadByte() override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count) override;
		virtual void WriteByte(Byte value) override;

		virtual void* Data() override {
			return _fstream.rdbuf();
		}

	public:
		std::fstream _fstream;

	private:
		std::streampos _filesize{ 0 };
		bool _closed{ false };
		bool _truncated{ false };

		Int endOfFile() {
			if (_closed)
				return 0;

			const auto pos = _fstream.tellg();
			_fstream.seekg(0, std::ios_base::end);

			const auto end = _fstream.tellg();
			_fstream.seekg(pos);

			return static_cast<Int>(end);
		}
	};
}

#endif