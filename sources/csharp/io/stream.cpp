#include "csharp/io/stream.hpp"
#include "csharp/io/exception.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <filesystem>
#include <string>

using byte = uint8_t;

namespace csharp {
	void Stream::CopyTo(Stream& destination, int32_t bufferLength)
	{
        if (!CanRead())
        {
            if (CanWrite())
            {
                throw NotSupportedException(SR::NotSupported_UnreadableStream);
            }

            throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
        }

        auto buffer = std::vector<byte>(bufferLength);

        int32_t bytesRead = 0;

        while ((bytesRead = Read(buffer.data(), static_cast<int32_t>(buffer.size()), 0, static_cast<int32_t>(buffer.size()))) != 0)
        {
            destination.Write(buffer.data(), static_cast<int32_t>(buffer.size()), 0, bytesRead);
        }
	}

    int32_t Stream::GetCopybufferLength() const 
    {
        constexpr auto DefaultCopybufferLength = 81920;
        auto bufferLength = DefaultCopybufferLength;

        if (CanSeek())
        {
            auto length = Length();
            auto position = Position();
            
            if (length <= position)
            {
                bufferLength = 1;
            }
            else
            {
                auto remaining = length - position;
                if (remaining > 0)
                {
                    bufferLength = static_cast<int32_t>(std::min(static_cast<int64_t>(bufferLength), remaining));
                }
            }
        }

        return bufferLength;
    }

    int32_t Stream::Read(uint8_t* buffer, int32_t bufferLength) {
        ValidateBuffer(buffer, bufferLength);

        auto sharedBuffer = std::vector<byte>(bufferLength);        
        auto numRead = Read(sharedBuffer.data(), bufferLength, 0, bufferLength);

        if (numRead > bufferLength)
        {
            throw IOException(SR::IO_StreamTooLong);
        }

        for (size_t i = 0; i < numRead; ++i)
            buffer[i] = sharedBuffer[i];
        
        return numRead;
    }

    int32_t Stream::ReadByte() {
        byte oneByteArray = 0;
        auto r = Read(&oneByteArray, 1, 0, 1);

        return r == 0 ? -1 : oneByteArray;
    }

    int32_t Stream::ReadAtLeastCore(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream) {
        ValidateBuffer(buffer, bufferLength);
        
        int32_t totalRead = 0;
        while (totalRead < minimumBytes)
        {
            auto read = Read(buffer, bufferLength);
            if (read == 0)
            {
                if (throwOnEndOfStream)
                {
                    throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
                }

                return totalRead;
            }

            totalRead += read;
        }

        return totalRead;
    }

    void Stream::Write(uint8_t const* buffer, int32_t bufferLength) {
        ValidateBuffer(buffer, bufferLength);

        auto sharedBuffer = std::vector<byte>(bufferLength);

        for (size_t i = 0; i < bufferLength; ++i)
            sharedBuffer[i] = buffer[i];

        Write(sharedBuffer.data(), bufferLength, 0, bufferLength);
    }

    void Stream::ValidateBuffer(uint8_t const* buffer, int32_t bufferLength) {
        if (!buffer) {
            throw ArgumentNullException("buffer");
        }

        if (bufferLength < 0) {
            throw ArgumentException("bufferLength");
        }
    }

    //
    //----------------------------------------------------------------
    // MemoryStream
    //----------------------------------------------------------------
    //

    void MemoryStream::EnsureNotClosed() const {
        if (!_isOpen)
            throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
    }

    void MemoryStream::EnsureWriteable() const {
        if (!CanWrite())
            throw NotSupportedException(SR::NotSupported_UnwritableStream);
    }

    bool MemoryStream::EnsureCapacity(int32_t value) {
        if (value < 0)
            throw IOException(SR::IO_StreamTooLong);

        if (value > _capacity)
        {
            auto newCapacity = std::max(value, 256);
            
            if (newCapacity < _capacity * 2)
            {
                newCapacity = _capacity * 2;
            }           
            

            if ((_capacity * 2) > MemStreamMaxLength)
            {
                newCapacity = std::max(value, MemStreamMaxLength);
            }

            Capacity(newCapacity);

            return true;
        }
        return false;
    }

    std::vector<uint8_t>& MemoryStream::GetBuffer() {
        if (!_exposable) {
            throw UnauthorizedAccessException(SR::UnauthorizedAccess_MemStreamBuffer);
        }        

        return _buffer;
    }

    bool MemoryStream::TryGetBuffer(std::vector<uint8_t>& buffer) {
        if (!_exposable)
        {
            return false;
        }

        buffer = _buffer;
        return true;
    }

    int32_t MemoryStream::Capacity() const {
        EnsureNotClosed();
        return _capacity - _origin;
    }

    void MemoryStream::Capacity(int32_t value) {
        if (value < Length())
            throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_SmallCapacity);

        EnsureNotClosed();

        if (!_expandable && (value != Capacity()))
            throw NotSupportedException(SR::NotSupported_MemStreamNotExpandable);
        
        if (_expandable && value != _capacity)
        {
            if (value > 0)
            {
                auto newBuffer = std::vector<byte>(value);

                if (_length > 0)
                {                    
                    //Buffer.BlockCopy(_buffer, 0, newBuffer, 0, _length);
                    std::memmove(newBuffer.data(), _buffer.data(), _length);
                }

                _buffer = newBuffer;
            }
            else
            {
                _buffer = std::vector<byte>();
            }
            _capacity = value;
        }
    }

    int64_t MemoryStream::Length() const {
        EnsureNotClosed();
        return _length - _origin;
    }

    int64_t MemoryStream::Position() const {
        EnsureNotClosed();
        return _position - _origin;
    }

    void MemoryStream::Position(int64_t value) {
        EnsureNotClosed();

        if (value > MemStreamMaxLength - _origin)
            throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);
        
        _position = static_cast<int32_t>(_origin + value);
    }

    int32_t MemoryStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
        ValidateBuffer(buffer, bufferLength);
        EnsureNotClosed();

        auto n = _length - _position;

        if (n > count)
            n = count;
        if (n <= 0)
            return 0;       

        if (n <= 8)
        {
            auto byteCount = n;
            while (--byteCount >= 0)
                buffer[offset + byteCount] = _buffer[_position + byteCount];
        }
        else {
            //Buffer.BlockCopy(_buffer, _position, buffer, offset, n);
            std::memmove(buffer + offset, _buffer.data() + _position, n);
        }

        _position += n;

        return static_cast<int32_t>(n);
    }

    int32_t MemoryStream::Read(uint8_t* buffer, int32_t bufferLength) {
        ValidateBuffer(buffer, bufferLength);
        EnsureNotClosed();

        int n = std::min(_length - _position, bufferLength);
        
        if (n <= 0)
            return 0;

        for (size_t i = 0; i < n; ++i)
            buffer[i] = _buffer[i];        

        _position += n;

        return n;
    }

    int32_t MemoryStream::ReadByte() {
        EnsureNotClosed();

        if (_position >= _length)
            return -1;

        return _buffer[_position++];
    }

    void MemoryStream::CopyTo(Stream& destination, int32_t bufferLength) {        
        EnsureNotClosed();

        auto originalPosition = _position;
                
        auto remaining = InternalEmulateRead(_length - originalPosition);        

        if (remaining > 0)
        {            
            destination.Write(_buffer.data(), static_cast<int32_t>(_buffer.size()), originalPosition, remaining);
        }
    }

    int32_t MemoryStream::InternalEmulateRead(int32_t count) {
        EnsureNotClosed();

        auto n = _length - _position;
        
        if (n > count)
            n = count;
        if (n < 0)
            n = 0;        
        _position += n;
        return n;
    }

    int64_t MemoryStream::Seek(int64_t offset, SeekOrigin loc) {
        EnsureNotClosed();
        
        int32_t _loc = 0;

        switch (loc)
        {
        case csharp::SeekOrigin::Begin:
            _loc = _origin;
            break;
        case csharp::SeekOrigin::Current:
            _loc = _position;
            break;
        case csharp::SeekOrigin::End:
            _loc = _length;
            break;
        default:
            throw ArgumentException(SR::Argument_InvalidSeekOrigin);
            break;
        }

        return SeekCore(offset, _loc);
    }

    int64_t MemoryStream::SeekCore(int64_t offset, int32_t loc) {
        if (offset > MemStreamMaxLength - loc)
            throw ArgumentOutOfRangeException("offset", SR::ArgumentOutOfRange_StreamLength);

        auto tempPosition = loc + static_cast<int32_t>(offset);

        if ((loc + offset) < _origin || tempPosition < _origin)
            throw IOException(SR::IO_SeekBeforeBegin);
       
        _position = tempPosition;
        
        return _position - _origin;
    }

    void MemoryStream::SetLength(int64_t value) {
        if (value < 0 || value > MemStreamMaxLength)
            throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);

        EnsureWriteable();
        
        if (value > (MemStreamMaxLength - _origin))
            throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);

        int newLength = _origin + (int)value;
        bool allocatedNewArray = EnsureCapacity(newLength);
        
        if (!allocatedNewArray && newLength > _length)
        {
            //Array.Clear(_buffer, _length, newLength - _length);
            for (size_t i = _length; i < newLength - _length; ++i) {
                _buffer[i] = 0;
            }
        }            

        _length = newLength;

        if (_position > newLength)
            _position = newLength;
    }

    void MemoryStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
        ValidateBuffer(buffer, bufferLength);
        EnsureNotClosed();
        EnsureWriteable();

        auto i = _position + count;
        
        if (i < 0)
            throw IOException(SR::IO_StreamTooLong);

        if (i > _length)
        {
            auto mustZero = _position > _length;

            if (i > _capacity)
            {
                bool allocatedNewArray = EnsureCapacity(i);
                if (allocatedNewArray)
                {
                    mustZero = false;
                }
            }
            if (mustZero)
            {
                //Array.Clear(_buffer, _length, i - _length);
                for (size_t j = _length; j < i - _length; ++j) {
                    _buffer[j] = 0;
                }
            }

            _length = i;
        }

        if ((count <= 8) && (buffer != _buffer.data()))
        {
            auto byteCount = count;
            while (--byteCount >= 0)
            {
                _buffer[_position + byteCount] = buffer[offset + byteCount];
            }
        }
        else
        {            
            //Buffer.BlockCopy(buffer, offset, _buffer, _position, count);
            std::memmove(_buffer.data() + _position, buffer + offset, count);
        }
        _position = i;
    }

    void MemoryStream::Write(uint8_t const* buffer, int32_t bufferLength) {
        EnsureNotClosed();
        EnsureWriteable();

        // Check for overflow
        auto i = _position + bufferLength;
        
        if (i < 0)
            throw IOException(SR::IO_StreamTooLong);

        if (i > _length)
        {
            auto mustZero = _position > _length;
            
            if (i > _capacity)
            {
                auto allocatedNewArray = EnsureCapacity(i);
                if (allocatedNewArray)
                {
                    mustZero = false;
                }
            }
            
            if (mustZero)
            {
                //Array.Clear(_buffer, _length, i - _length);
                for (size_t j = _length; j < i - _length; ++j) {
                    _buffer[j] = 0;
                }
            }

            _length = i;
        }
        
        //TODO: Verificar se está correto e verificar os std::memmove
        //buffer.CopyTo(new Span<byte>(_buffer, _position, buffer.Length));
        for (size_t x = _position; x < bufferLength; ++i) {
            _buffer[x] = buffer[x];
        }

        _position = i;
    }

    void MemoryStream::WriteByte(uint8_t value) {
        EnsureNotClosed();
        EnsureWriteable();

        if (_position >= _length)
        {
            auto newLength = _position + 1;
            auto mustZero = _position > _length;

            if (newLength >= _capacity)
            {
                bool allocatedNewArray = EnsureCapacity(newLength);
                if (allocatedNewArray)
                {
                    mustZero = false;
                }
            }
            if (mustZero)
            {
                //Array.Clear(_buffer, _length, _position - _length);
                for (size_t j = _length; j < _position - _length; ++j) {
                    _buffer[j] = 0;
                }
            }
            _length = newLength;
        }
        _buffer[_position++] = value;
    }

    void MemoryStream::WriteTo(Stream& stream) {
        EnsureNotClosed();        

        stream.Write(_buffer.data(), static_cast<int32_t>(_buffer.size()), _origin, _length - _origin);
    }

    //
    //----------------------------------------------------------------
    // FileStream
    //----------------------------------------------------------------
    //

    FileStream::FileStream(std::string const path, FileMode mode, FileShare shared, int32_t bufferLength) {
        auto flags = std::fstream::in
            | std::fstream::out
            | std::fstream::binary;

        const auto exists = std::filesystem::exists(path);

        switch (mode)
        {
            //Especifica se deve abrir um arquivo existente.
        case FileMode::Open:
            if (!exists)
                throw InvalidOperationException("The specified file does not exist.");
            break;
            //Especifica que se deve abrir um arquivo, se existir;
            // caso contrário, um novo arquivo deverá ser criado.
        case FileMode::OpenOrCreate:
        case FileMode::Create:
            if (!exists)
                flags |= std::fstream::trunc;
            break;
            //Especifica que o sistema operacional deve criar um novo arquivo.
            //Se o arquivo já existir, não abre o arquivo.
        case FileMode::CreateNew:
            if (!exists)
                flags |= std::fstream::trunc;
            else
                throw InvalidOperationException("The specified file already exists.");
            break;
            //Abre o arquivo, se existir, e busca o final do arquivo ou cria um novo arquivo.
        case FileMode::Append:
            if (!exists)
                flags |= std::fstream::trunc;
            else
                flags |= std::fstream::app;
            break;
            //Especifica que se deve abrir um arquivo existente.
            //Quando o arquivo for aberto, ele deverá ser truncado
            //para que seu tamanho seja zero bytes.
        case FileMode::Truncate:
            if (!exists)
                throw InvalidOperationException("The specified file does not exist.");

            flags |= std::fstream::trunc;
            break;
        default:
            throw InvalidOperationException();
            break;
        }

        stream.open(path.c_str(), flags);

        if (!stream.good())
            throw InvalidOperationException("Failed to open file: " + path);

        SetStreamLength();
        _position = stream.tellg();
    }

    void FileStream::SetStreamLength() {
        const auto pos = stream.tellg();
        stream.seekg(0, std::ios_base::end);

        const auto end = stream.tellg();
        stream.seekg(pos);

        _length = end;
    }    

    int64_t FileStream::Length() const {
        EnsureNotClosed();
        return static_cast<int64_t>(_length);
    }

    void FileStream::EnsureNotClosed() const {
        if (!stream.is_open())
            throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
    }

    int64_t FileStream::Position() const {
        EnsureNotClosed();
        return static_cast<int64_t>(_position);
    }

    void FileStream::Position(int64_t value) {
        EnsureNotClosed();
        const auto _position = static_cast<std::streampos>(value);
        stream.seekg(_position);
    }

    void FileStream::CopyTo(Stream& destination, int32_t bufferLength) {
        if (!CanRead())
        {
            if (CanWrite())
            {                
                throw NotSupportedException(SR::NotSupported_UnreadableStream);
            }
            
            throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
        }

        auto buffer = std::vector<byte>(bufferLength);
        int32_t bytesRead = 0;

        while ((bytesRead = Read(buffer.data(), bufferLength, 0, bufferLength)) != 0)
        {
            destination.Write(buffer.data(), bufferLength, 0, bytesRead);
        }
    }

    void FileStream::Close() {
        if (!stream.is_open())
            return;
        
        stream.close();
        _position = 0;
        _length = 0;
    }

    int64_t FileStream::Seek(int64_t offset, SeekOrigin origin) {
        EnsureNotClosed();
        stream.seekg(static_cast<std::streamoff>(offset), static_cast<int>(origin));
        _position = stream.tellg();
        return static_cast<int64_t>(_position);
    }

    void FileStream::SetLength(int64_t value) {
        EnsureNotClosed();
        EnsureWriteable();

        throw NotSupportedException();
    }

    void FileStream::EnsureWriteable() const {
        if (!CanWrite())
            throw NotSupportedException(SR::NotSupported_UnwritableStream);
    }

    int32_t FileStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
        EnsureNotClosed();
        ValidateBuffer(buffer, bufferLength);

        auto buff = reinterpret_cast<char*>(buffer);
        stream.read(buff + offset, count);

        if (stream.rdstate() != std::fstream::goodbit) {
            return -1;
        }

        return static_cast<int32_t>(stream.gcount());
    }

    int32_t FileStream::Read(uint8_t* buffer, int32_t bufferLength) {
        return Read(buffer, bufferLength, 0, bufferLength);
    }

    int32_t FileStream::ReadByte() {
        EnsureNotClosed(); 

        char c = 0;

        stream.read(&c, 1);

        if (stream.rdstate() != std::fstream::goodbit) {
            return -1;
        }

        const auto uchar = static_cast<unsigned char>(c);
        const auto result = static_cast<int32_t>(uchar);

        return result;
    }

    void FileStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
        EnsureNotClosed();
        EnsureWriteable();

        auto buff = reinterpret_cast<const char*>(buffer);

        stream.write(buff + offset, count);

        if (stream.rdstate() != std::fstream::goodbit) {
            throw InvalidOperationException();
        }

        SetStreamLength();
    }

    void FileStream::Write(uint8_t const* buffer, int32_t bufferLength) {
        Write(buffer, bufferLength, 0, bufferLength);
    }

    void FileStream::WriteByte(uint8_t value) {
        EnsureNotClosed();
        EnsureWriteable();

        const char c = static_cast<char>(value);

        stream.write(&c, 1);

        if (stream.rdstate() != std::fstream::goodbit) {
            throw InvalidOperationException();
        }

        SetStreamLength();
    }

    std::fstream& FileStream::GetBuffer() {
        return stream;
    }
}