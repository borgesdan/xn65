#ifndef XNA_CSHARP_STREAM_HPP
#define XNA_CSHARP_STREAM_HPP

#include "../types.hpp"
#include "../enums.hpp"
#include "../xnaerror.hpp"
#include <fstream>
#include <filesystem>

namespace xna {
	class Stream {
	public:
		virtual Int Length() = 0;
		virtual Long Position() = 0;
		virtual void Close() = 0;
		virtual Long Seek(Long offset, SeekOrigin const& origin, xna_error_nullarg) = 0;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) = 0;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_nullarg) = 0;
		virtual Int ReadByte(xna_error_nullarg) = 0;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) = 0;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_nullarg) = 0;
		virtual void WriteByte(Byte value, xna_error_nullarg) = 0;
	};

	class MemoryStream : public Stream {
	public:
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

		virtual Long Seek(Long offset, SeekOrigin const& origin, xna_error_ptr_arg) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_ptr_arg) override;
		virtual Int ReadByte(xna_error_ptr_arg) override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_ptr_arg) override;
		virtual void WriteByte(Byte value, xna_error_ptr_arg) override;

	public:
		Int _position{ 0 };
		Int _origin{ 0 };
		Int _length{ 0 };
		std::vector<Byte> _buffer;
		bool _closed{ false };
	};

	class FileStream : public Stream {
	public:
		FileStream(std::string const& path){
			int flags = std::fstream::in
				| std::fstream::out
				| std::fstream::binary
				| std::fstream::ate;

			const auto exists = std::filesystem::exists(path);

			if (!exists)
				flags |= std::fstream::trunc;

			_fstream.open(path.c_str(), flags);

			if (_fstream.is_open()) {
				_filesize = _fstream.tellg();
				_fstream.seekg(0);
			}
			else {
				_closed = true;
			}
		}

		~FileStream() {
			Close();
		}

		virtual constexpr Int Length() override {
			if (_closed)
				return 0;

			return static_cast<Int>(_filesize);
		}

		virtual Long Position() override {
			if (_closed)
				return 0;

			return static_cast<Long>(_fstream.tellg());
		}

		virtual void Close() override {
			_closed = true;

			if(_fstream.is_open())
				_fstream.close();
		}

		virtual Long Seek(Long offset, SeekOrigin const& origin, xna_error_ptr_arg) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_ptr_arg) override;
		virtual Int ReadByte(xna_error_ptr_arg) override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_ptr_arg) override;
		virtual void WriteByte(Byte value, xna_error_ptr_arg) override;

	public:
		std::streampos _filesize{ 0 };
		std::fstream _fstream;
		bool _closed{ false };
	};
}

#endif