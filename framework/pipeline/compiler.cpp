#include "xna/pipeline/compiler.hpp"

namespace xna {
	void ContentWriter::Write(Color const& value) {
		Write(value.PackedValue());		
	}

	void ContentWriter::Write(Quaternion const& value) {
		Write(value.X);
		Write(value.Y);
		Write(value.Z);
		Write(value.W);
	}

	void ContentWriter::Write(Matrix const& value) {
        Write(value.M11);
        Write(value.M12);
        Write(value.M13);
        Write(value.M14);
        Write(value.M21);
        Write(value.M22);
        Write(value.M23);
        Write(value.M24);
        Write(value.M31);
        Write(value.M32);
        Write(value.M33);
        Write(value.M34);
        Write(value.M41);
        Write(value.M42);
        Write(value.M43);
        Write(value.M44);
	}

    void ContentWriter::Write(Vector4 const& value) {
        Write(value.X);
        Write(value.Y);
        Write(value.Z);
        Write(value.W);
    }

    void ContentWriter::Write(Vector3 const& value) {
        Write(value.X);
        Write(value.Y);
        Write(value.Z);
    }

    void ContentWriter::Write(Vector2 const& value) {
        Write(value.X);
        Write(value.Y);
    }

    void ContentWriter::WriteVersionNumber(Ushort version) {
        version |= static_cast<Ushort>(static_cast<Int>(targetProfile) << 8 & 32512);
        Write(version);
    }

    void ContentWriter::WriteUncompressedOutput()
    {
        WriteVersionNumber(static_cast<Ushort>(5));
        const auto length1 = static_cast<Int>(headerData->Length());
        const auto length2 = static_cast<Int>(contentData->Length());
        Write(10 + length1 + length2);
        
        _stream->Write(headerData->_buffer, 0, length1);
        _stream->Write(contentData->_buffer, 0, length2);
    }

    void ContentWriter::WriteFinalOutput() {
        _stream = finalOutput;
        Write(static_cast<Byte>(88));
        Write(static_cast<Byte>(78));
        Write(static_cast<Byte>(66));

        //Target Platform Windows
        if (targetPlatform == TargetPlatform::Windows)
            Write(static_cast<Byte>(119));
        else
            Exception::Throw(ExMessage::InvalidOperation);
        
        if (compressContent)
            WriteCompressedOutput();
        else
            WriteUncompressedOutput();
    }

    void ContentWriter::WriteHeader() {
        _stream = reinterpret_pointer_cast<Stream>(headerData);

        Write7BitEncodedInt(typeWriters.size());

        for (size_t i = 0; i < typeWriters.size(); ++i) {
            const auto& typeWriter = typeWriters[i];
            
            Write(typeWriter->GetRuntimeReader(targetPlatform));
            Write(typeWriter->TypeVersion());
        }
        
        Write7BitEncodedInt(sharedResourceNames.size());
    }    
}