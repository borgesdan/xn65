#ifndef XNA_CSHARP_STREAM_HPP
#define XNA_CSHARP_STREAM_HPP

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

namespace xna {
	// Provides seek reference points.  
	// To seek to the end of a stream, call stream.Seek(0, SeekOrigin.End).
	enum class SeekOrigin {
		Begin,
		Current,
		End,
	};

	// Contains constants for specifying how the OS should open a file.
    // These will control whether you overwrite a file, open an existing
    // file, or some combination thereof.
    //
    // To append to a file, use Append (which maps to OpenOrCreate then we seek
    // to the end of the file).  To truncate a file or create it if it doesn't
    // exist, use Create.
	enum class FileMode {
		// Creates a new file. An exception is raised if the file already exists.
		CreateNew,
		// Creates a new file. If the file already exists, it is overwritten.
		Create,
		// Opens an existing file. An exception is raised if the file does not exist.
		Open,
		// Opens the file if it exists. Otherwise, creates a new file.
		OpenOrCreate,
		// Opens an existing file. Once opened, the file is truncated so that its
		// size is zero bytes. The calling process must open the file with at least
		// WRITE access. An exception is raised if the file does not exist.
		Truncate,
		// Opens the file if it exists and seeks to the end.  Otherwise,
		// creates a new file.
		Append,		
	};

	//A simplified port of the System.IO.Stream.
	//Provides a generic view of a sequence of bytes. This is an abstract class.
	class Stream {
	public:
		virtual ~Stream(){}

		//When overridden in a derived class, gets a value indicating whether the current stream supports reading.
		virtual bool CanRead() { return true; }
		//When overridden in a derived class, gets a value indicating whether the current stream supports writing.
		virtual bool CanWrite() { return true; }
		//When overridden in a derived class, gets a value indicating whether the current stream supports seeking.
		virtual bool CanSeek() { return true; }
		//Gets a value that determines whether the current stream can time out.
		virtual bool CanTimeout() { return false; }

		//Gets the length in bytes of the stream.
		virtual int64_t Length() = 0;
		//Gets or sets the position within the current stream.
		virtual int64_t Position() = 0;
		//Gets or sets the position within the current stream.
		virtual void Position(int64_t value) {	Seek(value, SeekOrigin::Begin); }

		//When overridden in a derived class, sets the length of the current stream.
		virtual void SetLength(int64_t value) = 0;

		//Closes the current stream and releases any resources
		virtual void Close() = 0;
		//Sets the position within the current stream.
		virtual int64_t Seek(int64_t offset, SeekOrigin const& origin) = 0;
		
		//Reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		//Reads a sequence of bytes from the current stream and advances the position within the stream by the number of bytes read.
		virtual int32_t Read(std::vector<uint8_t>& buffer, int32_t offset, int32_t count) = 0;
		
		//Reads a byte from the stream and advances the position within the stream by one byte, or returns -1 if at the end of the stream.
		virtual int32_t ReadByte() = 0;
		
		//Writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
		virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		//Writes a sequence of bytes to the current stream and advances the current position within this stream by the number of bytes written.
		virtual void Write(std::vector<uint8_t> const& buffer, int32_t offset, int32_t count) = 0;
		
		//Writes a byte to the current position in the stream and advances the position within the stream by one byte.
		virtual void WriteByte(uint8_t value) = 0;		
	};

	//A simplified port of the System.IO.MemoryStream.
	class MemoryStream : public Stream {
	public:
		constexpr MemoryStream(){}

		constexpr MemoryStream(std::vector<uint8_t> const& bytes):
			_buffer(bytes), 
			_length(static_cast<int64_t>(bytes.size())){}

		~MemoryStream() override {
			Close();
		}

		constexpr MemoryStream(int32_t capacity) :
			_buffer(static_cast<size_t>(capacity)),
			_length(capacity > 0 ? capacity : 0){}

		virtual constexpr int64_t Length() override {
			if (_closed)
				return 0;

			return _length;
		}

		virtual constexpr void SetLength(int64_t value) override {
			_buffer.reserve(value);
			_length = value;
		}

		virtual constexpr int64_t Position() override {
			if (_closed)
				return 0;

			return _position;
		}

		virtual constexpr void Close() override {
			_closed = true;
			_buffer = std::vector<uint8_t>();
			
		}		

		virtual int64_t Seek(int64_t offset, SeekOrigin const& origin) override;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		virtual int32_t Read(std::vector<uint8_t>& buffer, int32_t offset, int32_t count) override;
		virtual int32_t ReadByte() override;
		virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		virtual void Write(std::vector<uint8_t> const& buffer, int32_t offset, int32_t count) override;
		virtual void WriteByte(uint8_t value) override;

	public:
		std::vector<uint8_t> _buffer;

	private:
		int32_t _position{ 0 };
		int32_t _origin{ 0 };
		int64_t _length{ 0 };
		bool _closed{ false };
	};

	//A simplified port of the System.IO.FileStream.
	class FileStream : public Stream {
	public:
		FileStream(std::string const& path, FileMode fileMode);
		FileStream(std::string const& path);

		~FileStream() override {
			Close();
		}		

		virtual int64_t Length() override;
		virtual int64_t Position() override;

		virtual constexpr void SetLength(int64_t value) override {
			Exception::Throw(Exception::NOT_IMPLEMENTED);
		}

		inline virtual void Close() override {
			_closed = true;

			if(_fstream.is_open())
				_fstream.close();
		}		

		virtual int64_t Seek(int64_t offset, SeekOrigin const& origin) override;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		virtual int32_t Read(std::vector<uint8_t>& buffer, int32_t offset, int32_t count) override;
		virtual int32_t ReadByte() override;
		virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		virtual void Write(std::vector<uint8_t> const& buffer, int32_t offset, int32_t count) override;
		virtual void WriteByte(uint8_t value) override;		

	public:
		std::fstream _fstream;

	private:
		std::streampos _filesize{ 0 };
		bool _closed{ false };
		bool _truncated{ false };

		int64_t endOfFile() {
			if (_closed)
				return 0;

			const auto pos = _fstream.tellg();
			_fstream.seekg(0, std::ios_base::end);

			const auto end = _fstream.tellg();
			_fstream.seekg(pos);

			return static_cast<int64_t>(end);
		}
	};
}

#endif