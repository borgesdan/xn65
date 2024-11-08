#include "xna/csharp/stream.hpp"
#include "xna/csharp/buffer.hpp"
#include "xna/exception.hpp"
#include <filesystem>

namespace xna {
	int64_t MemoryStream::Seek(int64_t offset, SeekOrigin const& origin) {
		int64_t p = 0;

		switch (origin)
		{
		case SeekOrigin::Begin:
			p = _origin + offset;
			
			if (p < _origin) {
				return -1;
			}			
			break;
		case SeekOrigin::Current:
			p = _position + offset;

			if (p < _origin) {
				return -1;
			}			
			break;
		case SeekOrigin::End:
			p = _length + offset;

			if (p < _origin) {
				return -1;
			}			
			break;
		default:
			return -1;
		}

		_position = static_cast<int32_t>(p);
		return _position;
	}

	int32_t MemoryStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {			
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
			Buffer::BlockCopy<uint8_t>(_buffer.data(), _position, buffer, offset, off);
		}			
		
		_position += off;

		return off;
	}

	int32_t MemoryStream::Read(std::vector<uint8_t>& buffer, int32_t offset, int32_t count) {
		return Read(buffer.data(), static_cast<int32_t>(buffer.size()), offset, count);
	}

	int32_t MemoryStream::ReadByte() {
		if (!_closed)
			return 0;

		if (_position >= _length)
			return -1;

		return _buffer[_position++];
	}

	void MemoryStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count){
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {						
			return;
		}

		if (_closed)
			return;

		auto i = _position + count;

		if (i < 0 || i > _length) {
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

	void MemoryStream::Write(std::vector<uint8_t> const& buffer, int32_t offset, int32_t count){
		Write(buffer.data(), static_cast<int32_t>(buffer.size()), offset, count);
	}

	void MemoryStream::WriteByte(uint8_t value) {
		if (_closed)
			return;

		if (_position >= _length) {
			return;
		}

		_buffer[_position++] = value;
	}

	FileStream::FileStream(std::string const& path, FileMode fileMode) {
		int flags = std::fstream::in
			| std::fstream::out
			| std::fstream::binary;

		const auto exists = std::filesystem::exists(path);

		switch (fileMode)
		{
			//Especifica se deve abrir um arquivo existente.
		case FileMode::Open:
			if (!exists) 
				Exception::Throw("The specified file does not exist.");
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
				Exception::Throw("The specified file already exists.");
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
			//Tentativa de ler um arquivo truncado retornará 0;
		case FileMode::Truncate:
			if(!exists)
				Exception::Throw("The specified file does not exist.");

			flags |= std::fstream::trunc;
			_truncated = true;
			break;
		default:
			break;
		}

		_fstream.open(path.c_str(), flags);

		if (!_fstream.good())
			Exception::Throw("Failed to open file: " + path);
	}

	FileStream::FileStream(std::string const& path) {
		int flags = std::fstream::in
			| std::fstream::out
			| std::fstream::binary;

		const auto exists = std::filesystem::exists(path);

		if (!exists)
			flags |= std::fstream::trunc;

		_fstream.open(path.c_str(), flags);

		if (!_fstream.good())
			Exception::Throw("Failed to open file: " + path);
	}

	int64_t FileStream::Length() {
		if (_closed)
			return 0;

		const auto end = endOfFile();
		return end;
	}

	int64_t FileStream::Position() {
		if (_closed)
			return 0;

		return static_cast<int64_t>(_fstream.tellg());
	}

	int64_t FileStream::Seek(int64_t offset, SeekOrigin const& origin){
		if (_closed)
			return 0;

		int seek;

		switch (origin)
		{
		case SeekOrigin::Begin:
			seek = std::ios_base::beg;
			break;
		case SeekOrigin::Current:
			seek = std::ios_base::cur;
			break;
		case SeekOrigin::End:
			seek = std::ios_base::end;
			break;
		default:
			return -1;
		}
		
		_fstream.seekg(offset, seek);				
		const auto state = _fstream.rdstate();

		if (state != std::fstream::goodbit) {
			return -1;
		}		

		const auto pos = static_cast<int64_t>(_fstream.tellg());
		return pos;
	}

	int32_t FileStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count){
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {			
			return -1;
		}
		
		if (_closed || _truncated)
			return 0; 

		auto _buff = reinterpret_cast<char*>(buffer);
		_fstream.read(_buff + offset, count);

		if (_fstream.rdstate() != std::fstream::goodbit) {
			return -1;
		}

		return static_cast<int32_t>(_fstream.gcount());
	}

	int32_t FileStream::Read(std::vector<uint8_t>& buffer, int32_t offset, int32_t count){
		return Read(buffer.data(), static_cast<int32_t>(buffer.size()), offset, count);
	}

	int32_t FileStream::ReadByte(){
		if (_closed || _truncated)
			return 0;

		char c = 0;

		_fstream.read(&c, 1);

		if (_fstream.rdstate() != std::fstream::goodbit) {
			return -1;
		}

		const auto result = static_cast<int32_t>(c);

		return result;
	}

	void FileStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		if (buffer == nullptr || offset < 0 || count < 0 || bufferLength - offset < count) {
			return;
		}
		
		if (_closed)
			return;

		auto _buff = reinterpret_cast<const char*>(buffer);

		_fstream.write(_buff + offset, count);

		if (_fstream.rdstate() != std::fstream::goodbit) {
			return;
		}
	}

	void FileStream::Write(std::vector<uint8_t> const& buffer, int32_t offset, int32_t count) {
		Write(buffer.data(), static_cast<int32_t>(buffer.size()), offset, count);
	}

	void FileStream::WriteByte(uint8_t value) {
		if (_closed)
			return;

		const char c = static_cast<char>(value);

		_fstream.write(&c, 1);

		if (_fstream.rdstate() != std::fstream::goodbit) {
			return;
		}
	}
}