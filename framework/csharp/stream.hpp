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
		virtual ~Stream(){}
		virtual Int Length() = 0;
		virtual Long Position() = 0;		
		virtual void Close() = 0;
		virtual bool IsClosed() = 0;
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

		virtual constexpr bool IsClosed() override {
			return _closed;
		}

		virtual Long Seek(Long offset, SeekOrigin const& origin, xna_error_nullarg) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_nullarg) override;
		virtual Int ReadByte(xna_error_nullarg) override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_nullarg) override;
		virtual void WriteByte(Byte value, xna_error_nullarg) override;

	public:
		Int _position{ 0 };
		Int _origin{ 0 };
		Int _length{ 0 };
		std::vector<Byte> _buffer;
		bool _closed{ false };
	};

	class FileStream : public Stream {
	public:
		FileStream(String const& path, FileMode fileMode) {
			int flags = std::fstream::in
				| std::fstream::out
				| std::fstream::binary;

			const auto exists = std::filesystem::exists(path);

			switch (fileMode)
			{
			//Especifica se deve abrir um arquivo existente.
			case FileMode::Open:				
				if (!exists) {
					_closed = true;
					return;
				}
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
					return;
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
				flags |= std::fstream::trunc;
				_truncated = true;
				break;
			default:
				break;
			}

			_fstream.open(path.c_str(), flags);

			if (!_fstream.good())
				_closed = true;
		}

		FileStream(String const& path){
			int flags = std::fstream::in
				| std::fstream::out
				| std::fstream::binary;
				//| std::fstream::ate;

			const auto exists = std::filesystem::exists(path);

			if (!exists)
				flags |= std::fstream::trunc;

			_fstream.open(path.c_str(), flags);

			if (!_fstream.good())
				_closed = true;
		}

		~FileStream() {
			Close();
		}		

		virtual Int Length() override {
			if (_closed)
				return 0;

			const auto end = endOfFile();
			return end;
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

		virtual constexpr bool IsClosed() override {
			return _closed;
		}

		virtual Long Seek(Long offset, SeekOrigin const& origin, xna_error_nullarg) override;
		virtual Int Read(Byte* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		virtual Int Read(std::vector<Byte>& buffer, Int offset, Int count, xna_error_nullarg) override;
		virtual Int ReadByte(xna_error_nullarg) override;
		virtual void Write(Byte const* buffer, Int bufferLength, Int offset, Int count, xna_error_nullarg) override;
		virtual void Write(std::vector<Byte> const& buffer, Int offset, Int count, xna_error_nullarg) override;
		virtual void WriteByte(Byte value, xna_error_nullarg) override;

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