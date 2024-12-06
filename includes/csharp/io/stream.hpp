#ifndef CSHARP_IO_STREAM_HPP
#define CSHARP_IO_STREAM_HPP

#include "../exception.hpp"
#include <cstdint>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

namespace csharp {
	// Provides seek reference points.  To seek to the end of a stream,
   // call stream.Seek(0, SeekOrigin.End).
	enum class SeekOrigin
	{
		// These constants match Win32's FILE_BEGIN, FILE_CURRENT, and FILE_END
		Begin = 0,
		Current = 1,
		End = 2,
	};

	class Stream {
	public:
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		virtual bool CanSeek() const = 0;
		virtual bool CanTimeout() const { return false; }
		virtual int64_t Length() const = 0;
		virtual int64_t Position() const = 0;
		virtual void Position(int64_t value) = 0;

		virtual int32_t ReadTimeout() {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
			return 0;
		}

		virtual void ReadTimeout(int32_t value) {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
		}

		virtual int32_t WriteTimeout() {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
			return 0;
		}

		virtual void WriteTimeout(int32_t value) {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
		}

		void CopyTo(Stream& destination) {
			CopyTo(destination, GetCopybufferLength());
		}

		virtual void CopyTo(Stream& destination, int32_t bufferLength);
		virtual void Close() {}
		virtual void Flush() = 0;
		virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
		virtual void SetLength(int64_t value) = 0;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);
		virtual int32_t ReadByte();

		void ReadExactly(uint8_t* buffer, int32_t bufferLength) {
			ReadAtLeastCore(buffer, bufferLength, bufferLength, true);
		}

		virtual void ReadExactly(uint8_t* buffer, int32_t offset, int32_t count) {
			ReadAtLeastCore(buffer + offset, count, count, true);
		}

		int32_t ReadAtLeast(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream = true) {
			return  ReadAtLeastCore(buffer, bufferLength, minimumBytes, throwOnEndOfStream);
		}

		virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		virtual void Write(uint8_t const* buffer, int32_t bufferLength);
		virtual void WriteByte(uint8_t value) = 0;

	protected:
		void ValidateBuffer(uint8_t const* buffer, int32_t bufferLength);

	private:
		int32_t ReadAtLeastCore(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream);

	private:
		int32_t GetCopybufferLength() const;
	};

	class MemoryStream : public Stream {
	public:
		MemoryStream() : MemoryStream(0) {}

		MemoryStream(int32_t capacity)
		{
			_buffer.resize(static_cast<size_t>(capacity));
			_capacity = capacity;
			_expandable = true;
			_writable = true;
			_exposable = true;
			_isOpen = true;
		}

		MemoryStream(std::vector<uint8_t> const buffer)
			: MemoryStream(buffer, true) {}

		MemoryStream(std::vector<uint8_t> const buffer, bool writable) {
			_buffer = buffer;
			_length = _capacity = static_cast<int32_t>(buffer.size());
			_writable = writable;
			_isOpen = true;
		}

		MemoryStream(std::vector<uint8_t> const buffer, int32_t index, int32_t count)
			: MemoryStream(buffer, index, count, true, false) {}

		MemoryStream(std::vector<uint8_t> const buffer, int32_t index, int32_t count, bool writable)
			: MemoryStream(buffer, index, count, writable, false) {}

		MemoryStream(std::vector<uint8_t> const buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible)
		{
			if (index < 0)
				index = 0;

			if (count < 0)
				count = 0;

			if (buffer.size() - static_cast<size_t>(index < count))
				throw ArgumentException(SR::Argument_InvalidOffLen);

			_buffer = buffer;
			_origin = _position = index;
			_length = _capacity = index + count;
			_writable = writable;
			_exposable = publiclyVisible;
			_isOpen = true;
		}

		bool CanRead() const override { return _isOpen; }
		bool CanSeek() const override { return _isOpen; }
		bool CanWrite() const override { return _writable; }
		void Flush() override {}
		virtual std::vector<uint8_t>& GetBuffer();
		virtual bool TryGetBuffer(std::vector<uint8_t>& buffer);
		virtual int32_t Capacity() const;
		virtual void Capacity(int32_t value);
		int64_t Length() const override;
		int64_t Position() const override;
		void Position(int64_t value) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength) override;
		int32_t ReadByte() override;
		void CopyTo(Stream& destination, int32_t bufferLength) override;
		int32_t InternalEmulateRead(int32_t count);
		int64_t Seek(int64_t offset, SeekOrigin loc) override;
		void SetLength(int64_t value) override;
		void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		void Write(uint8_t const* buffer, int32_t bufferLength) override;
		void WriteByte(uint8_t value) override;
		virtual void WriteTo(Stream& stream);

		constexpr void Close() override {
			if (!_isOpen)
				return;

			_isOpen = false;
			_writable = false;
			_expandable = false;
			_buffer.clear();
		}

	private:
		void EnsureNotClosed() const;
		void EnsureWriteable() const;
		bool EnsureCapacity(int32_t value);
		int64_t SeekCore(int64_t offset, int32_t loc);

	public:
		std::vector<uint8_t> _buffer;

	private:
		int32_t _origin{ 0 };
		int32_t _position{ 0 };
		int32_t _length{ 0 };
		int32_t _capacity{ 0 };
		bool _expandable{ false };
		bool _writable{ false };
		bool _exposable{ false };
		bool _isOpen{ false };

		inline static constexpr int32_t MemStreamMaxLength = std::numeric_limits<int32_t>::max();
	};

	// Contains constants for specifying how the OS should open a file.
	// These will control whether you overwrite a file, open an existing
	// file, or some combination thereof.
	// To append to a file, use Append (which maps to OpenOrCreate then we seek
	// to the end of the file).  To truncate a file or create it if it doesn't
	// exist, use Create.
	enum class FileMode
	{
		// Creates a new file. An exception is raised if the file already exists.
		CreateNew = 1,
		// Creates a new file. If the file already exists, it is overwritten.
		Create = 2,
		// Opens an existing file. An exception is raised if the file does not exist.
		Open = 3,
		// Opens the file if it exists. Otherwise, creates a new file.
		OpenOrCreate = 4,
		// Opens an existing file. Once opened, the file is truncated so that its
		// size is zero bytes. The calling process must open the file with at least
		// WRITE access. An exception is raised if the file does not exist.
		Truncate = 5,
		// Opens the file if it exists and seeks to the end.  Otherwise,
		// creates a new file.
		Append = 6,
	};

	// Contains constants for specifying the access you want for a file.
	// You can have Read, Write or ReadWrite access.
	//
	enum class FileAccess
	{
		// Specifies read access to the file. Data can be read from the file and
		// the file pointer can be moved. Combine with WRITE for read-write access.
		Read = 1,

		// Specifies write access to the file. Data can be written to the file and
		// the file pointer can be moved. Combine with READ for read-write access.
		Write = 2,

		// Specifies read and write access to the file. Data can be written to the
		// file and the file pointer can be moved. Data can also be read from the
		// file.
		ReadWrite = 3,
	};

	// Contains constants for controlling file sharing options while
   // opening files.  You can specify what access other processes trying
   // to open the same file concurrently can have.
   //
   // Note these values currently match the values for FILE_SHARE_READ,
   // FILE_SHARE_WRITE, and FILE_SHARE_DELETE in winnt.h
   //
	enum class FileShare
	{
		// No sharing. Any request to open the file (by this process or another
		// process) will fail until the file is closed.
		None = 0,

		// Allows subsequent opening of the file for reading. If this flag is not
		// specified, any request to open the file for reading (by this process or
		// another process) will fail until the file is closed.
		Read = 1,

		// Allows subsequent opening of the file for writing. If this flag is not
		// specified, any request to open the file for writing (by this process or
		// another process) will fail until the file is closed.
		Write = 2,

		// Allows subsequent opening of the file for writing or reading. If this flag
		// is not specified, any request to open the file for writing or reading (by
		// this process or another process) will fail until the file is closed.
		ReadWrite = 3,

		// Open the file, but allow someone else to delete the file.
		Delete = 4,

		// Whether the file handle should be inheritable by child processes.
		// Note this is not directly supported like this by Win32.
		Inheritable = 0x10,
	};

	// Maps to FILE_FLAG_DELETE_ON_CLOSE and similar values from winbase.h.
	// We didn't expose a number of these values because we didn't believe
	// a number of them made sense in managed code, at least not yet.
	enum class FileOptions
	{
		// NOTE: any change to FileOptions enum needs to be
		// matched in the FileStream ctor for error validation
		None = 0,
		WriteThrough = static_cast<int32_t>(0x80000000),
		Asynchronous = static_cast<int32_t>(0x40000000), // FILE_FLAG_OVERLAPPED
		// NoBuffering = 0x20000000, // FILE_FLAG_NO_BUFFERING
		RandomAccess = 0x10000000,
		DeleteOnClose = 0x04000000,
		SequentialScan = 0x08000000,
		// AllowPosix = 0x01000000,  // FILE_FLAG_POSIX_SEMANTICS
		// BackupOrRestore = 0x02000000, // FILE_FLAG_BACKUP_SEMANTICS
		// DisallowReparsePoint = 0x00200000, // FILE_FLAG_OPEN_REPARSE_POINT
		// NoRemoteRecall = 0x00100000, // FILE_FLAG_OPEN_NO_RECALL
		// FirstPipeInstance = 0x00080000, // FILE_FLAG_FIRST_PIPE_INSTANCE
		Encrypted = 0x00004000, // FILE_ATTRIBUTE_ENCRYPTED
	};

	class FileStream : public Stream {
	public:
		FileStream(std::string const path, FileMode mode) : FileStream(path, mode, FileShare::None, 0) {}
		FileStream(std::string const path, FileMode mode, FileAccess access) : FileStream(path, mode, FileShare::None, 0) {}
		FileStream(std::string const path, FileMode mode, FileShare shared) : FileStream(path, mode, shared, 0) {}
		FileStream(std::string const path, FileMode mode, FileShare shared, int32_t bufferLength);

		constexpr bool CanRead() const override {
			return true;
		}

		constexpr bool CanWrite() const override {
			return true;
		}

		constexpr bool CanSeek() const override {
			return true;
		}

		int64_t Length() const override;
		int64_t Position() const override;
		void Position(int64_t value) override;
		void CopyTo(Stream& destination, int32_t bufferLength) override;		
		void Close() override;
		void Flush() override { stream.flush(); }
		int64_t Seek(int64_t offset, SeekOrigin origin) override;
		void SetLength(int64_t value) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength) override;
		int32_t ReadByte() override;
		void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		void Write(uint8_t const* buffer, int32_t bufferLength) override;
		void WriteByte(uint8_t value) override;

		virtual std::fstream& GetBuffer();

	private:
		void SetStreamLength();
		void EnsureNotClosed() const;
		void EnsureWriteable() const;

	public:
		std::fstream stream;

	private:
		std::streampos _length{ 0 };		
		std::streampos _position{ 0 };		
	};
}

#endif