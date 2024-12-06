#include "csharp/io/binary.hpp"
#include <vector>
#include <cstdint>

namespace csharp {
	int32_t BinaryReader::PeekChar() {
        if (_disposed)
            throw InvalidOperationException();

        if (!_stream->CanSeek())
        {
            return -1;
        }

        const auto origPos = _stream->Position();
        const auto ch = Read();
        
        _stream->Position(origPos);

        return ch;
	}

    int32_t BinaryReader::Read() {
        if (_disposed)
            throw InvalidOperationException();

        int32_t charsRead = 0;
        int32_t numBytes;
        int64_t posSav = 0;

        if (_stream->CanSeek())
        {
            posSav = _stream->Position();
        }

        auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);
        char singleChar = '\0';

        while (charsRead == 0) {
            numBytes = _2BytesPerChar ? 2 : 1;

            auto r = _stream->ReadByte();
            charBytes[0] = static_cast<uint8_t>(r);

            if (r == -1)
            {
                numBytes = 0;
            }
            if (numBytes == 2)
            {
                r = _stream->ReadByte();
                charBytes[1] = static_cast<uint8_t>(r);

                if (r == -1)
                {
                    numBytes = 1;
                }
            }

            if (numBytes == 0)
            {
                return -1;
            }

            const auto chars = reinterpret_cast<char*>(charBytes.data());
            const auto decoder = std::string(chars);
            charsRead = decoder.size();
            singleChar = decoder[0];
        }

        return singleChar;
    }

    uint8_t BinaryReader::InternalReadByte() {
        if (_disposed)
            throw InvalidOperationException();

        const auto b = _stream->ReadByte();
        if (b == -1)
        {
            throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
        }

        return static_cast<uint8_t>(b);
    }

    char BinaryReader::ReadChar() {
        const auto value = Read();

        if (value == -1)
        {
            throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
        }

        return static_cast<char>(value);
    }    

    void BinaryReader::InternalRead(std::vector<uint8_t>& buffer) {
        if (_disposed)
            throw InvalidOperationException();

        _stream->ReadExactly(buffer.data(), buffer.size());        
    }

    std::string BinaryReader::ReadString() {
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

        std::string sb;

        do
        {
            const auto readLength = std::min(MaxCharBytesSize, stringLength - currPos);
            const auto n = _stream->Read(charBytes.data(), readLength);

            if (n == 0)
            {
                throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
            }

            const auto chars = reinterpret_cast<char*>(charBytes.data());

            if (currPos == 0 && n == stringLength)
            {                
                return std::string(chars);
            }
            
            sb.append(chars);

            currPos += n;

        } while (currPos < stringLength);

        return sb;
    }    

    int32_t BinaryReader::Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count) {
        ArgumentNullException::ThrowIfNull(buffer, "buffer");

        if (index < 0)
            index = 0;

        if (count < 0)
            count = 0;

        if (bufferLength - index < count) {
            throw ArgumentException(SR::Argument_InvalidOffLen);
        }

        if (_disposed)
            throw InvalidOperationException();        

        return InternalReadChars(buffer + index, count);
    }

    int32_t BinaryReader::Read(char* buffer, int32_t bufferLength) {
        if (_disposed)
            throw InvalidOperationException();

        return InternalReadChars(buffer, bufferLength);
    }

    int32_t BinaryReader::InternalReadChars(char* buffer, int32_t bufferLength) {
        int totalCharsRead = 0;
        auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);
        
        auto numBytes = bufferLength;

        while (bufferLength > 0)
        {
            auto numBytes = bufferLength;
            
            if (_2BytesPerChar)
            {
                numBytes <<= 1;
            }

            std::vector<uint8_t> byteBuffer;
            
            if (numBytes > MaxCharBytesSize)
            {
                numBytes = MaxCharBytesSize;
            }

            numBytes = _stream->Read(charBytes.data(), numBytes);
            byteBuffer = std::vector<uint8_t>(charBytes.begin(), charBytes.begin() + numBytes);

            if (byteBuffer.empty())
            {
                break;
            }            

            auto chars = reinterpret_cast<char*>(byteBuffer.data());
            auto charsRead = std::string(chars);

            bufferLength = charsRead.length();            
            totalCharsRead += charsRead.length();
        }

        // we may have read fewer than the number of characters requested if end of stream reached
        // or if the encoding makes the char count too big for the buffer (e.g. fallback sequence)
        return totalCharsRead;
    }

    std::vector<char> BinaryReader::ReadChars(int32_t count) {
        if (count < 0)
            throw ArgumentOutOfRangeException();
        
        if (_disposed)
            throw InvalidOperationException();

        if (count == 0)
            return std::vector<char>();

        auto chars = std::vector<char>(count);
        const auto n = InternalReadChars(chars.data(), chars.size());

        if (n != count) {
            chars = std::vector<char>(chars.begin(), chars.begin() + count);
        }

        return chars;
    }

    int32_t BinaryReader::Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count) {
        ArgumentNullException::ThrowIfNull(buffer, "buffer");
        ArgumentOutOfRangeException::ThrowIfNegative(index, "index");
        ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

        if (bufferLength - index < count) {
            throw ArgumentException(SR::Argument_InvalidOffLen);
        }

        if (_disposed)
            throw InvalidOperationException();

        return _stream->Read(buffer, bufferLength, index, count);
    }

    int32_t BinaryReader::Read(uint8_t* buffer, int32_t bufferLength) {
        if (_disposed)
            throw InvalidOperationException();

        return _stream->Read(buffer, bufferLength);
    }

    std::vector<uint8_t> BinaryReader::ReadBytes(int32_t count) {
        ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

        if (_disposed)
            throw InvalidOperationException();

        if (count == 0)
            return std::vector<uint8_t>();

        auto  result = std::vector<uint8_t>(count);
        const auto numRead = _stream->ReadAtLeast(result.data(), result.size(), result.size(), false);

        if (numRead != result.size())
        {
            result = std::vector<uint8_t>(result.begin(), result.begin() + numRead);
        }

        return result;
    }

    void BinaryReader::ReadExactly(uint8_t* buffer, int32_t bufferLength) {
        if (_disposed)
            throw InvalidOperationException();

        _stream->ReadExactly(buffer, bufferLength);
    }

    int32_t BinaryReader::Read7BitEncodedInt() {
        uint32_t result = 0;
        uint8_t byteReadJustNow;

        constexpr int32_t MaxBytesWithoutOverflow = 4;

        for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
        {            
            byteReadJustNow = ReadByte();
            result |= (byteReadJustNow & 0x7Fu) << shift;

            if (byteReadJustNow <= 0x7Fu)
            {
                return static_cast<int32_t>(result);
            }
        }

        byteReadJustNow = ReadByte();
        
        if (byteReadJustNow > 15u)
        {
            //FormatException
            throw InvalidOperationException(SR::Format_Bad7BitInt);
        }

        result |= static_cast<uint32_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
        return static_cast<int32_t>(result);
    }

    int64_t BinaryReader::Read7BitEncodedInt64() {
        uint64_t result = 0;
        uint8_t byteReadJustNow;

        constexpr int32_t MaxBytesWithoutOverflow = 9;

        for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
        {
            byteReadJustNow = ReadByte();
            result |= (byteReadJustNow & 0x7Ful) << shift;

            if (byteReadJustNow <= 0x7Fu)
            {
                return static_cast<int64_t>(result);
            }
        }

        byteReadJustNow = ReadByte();
        
        if (byteReadJustNow > 1u)
        {
            //FormatException
            throw InvalidOperationException(SR::Format_Bad7BitInt);
        }

        result |= static_cast<int64_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
        return static_cast<int64_t>(result);
    }
}