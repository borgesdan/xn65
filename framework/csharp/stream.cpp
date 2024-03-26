#include "stream.hpp"
#include "buffer.hpp"

namespace xna {
	Long MemoryStream::Seek(Long offset, SeekOrigin const& origin, xna_error_ptr_arg) {
		Long p = 0;

		switch (origin)
		{
		case SeekOrigin::Begin:
			p = _origin + offset;
			
			if (p < _origin) {
				xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
				return -1;
			}			
			break;
		case SeekOrigin::Current:
			p = _position + offset;

			if (p < _origin) {
				xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
				return -1;
			}			
			break;
		case SeekOrigin::End:
			p = _length + offset;

			if (p < _origin) {
				xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
				return -1;
			}			
			break;
		default:
			xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
			return -1;
		}

		assertm(_position >= 0, "position >= 0");

		_position = static_cast<Int>(p);
		return _position;
	}

	Int MemoryStream::Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg) {
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return -1;
		}

		auto off = _length - _position;
		if (off > count) off = count;

		if (off <= 0 || _closed)
			return 0;

		if (off <= 8) {
			auto byteCount = off;
			while (--byteCount >= 0)
				buffer[offset + byteCount] = _buffer[_position + static_cast<size_t>(byteCount)];
		}
		else {
			Buffer::BlockCopy<Byte>(_buffer.data(), _position, buffer, offset, off);
		}			
		
		_position += off;

		return off;
	}

	Int MemoryStream::Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_ptr_arg) {
		return Read(buffer.data(), static_cast<Int>(buffer.size()), offset, count, err);
	}

	Int MemoryStream::ReadByte(xna_error_ptr_arg) {
		if (!_closed)
			return 0;

		if (_position >= _length)
			return -1;

		return _buffer[_position++];
	}

	void MemoryStream::Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_ptr_arg){
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);			
			return;
		}

		if (_closed)
			return;

		auto i = _position + count;

		if (i < 0 || i > _length) {
			xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
			return;
		}

		if (count <= 8)	{
			auto byteCount = count;
			while (--byteCount >= 0)
				_buffer[_position + static_cast<size_t>(byteCount)] = buffer[offset + byteCount];
		}
		else
			Buffer::BlockCopy(buffer, offset, _buffer.data(), _position, count);

		_position = i;
	}

	void MemoryStream::Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_ptr_arg){
		Write(buffer.data(), static_cast<Int>(buffer.size()), offset, count, err);
	}

	void MemoryStream::WriteByte(Byte value, xna_error_ptr_arg) {
		if (_closed)
			return;

		if (_position >= _length) {
			xna_error_apply(err, XnaErrorCode::OVERFLOW_OPERATION);
			return;
		}

		_buffer[_position++] = value;
	}
}