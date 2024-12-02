#include "pipeline/writer.hpp"
#include "pipeline/compiler.hpp"

namespace xna {

    //
    // ContentWriter
    //

    ContentWriter::ContentWriter(
        P_ContentCompiler& compiler,
        P_Stream const& output,
        TargetPlatform targetPlatform,
        GraphicsProfile targetProfile,
        bool compressContent,
        String const& rootDirectory,
        String const& referenceRelocationPath
    ) : compiler(compiler),
        targetPlatform(targetPlatform),
        targetProfile(targetProfile),
        compressContent(compressContent),
        rootDirectory(rootDirectory),
        referenceRelocationPath(referenceRelocationPath),
        finalOutput(output)
    {
        headerData = snew<MemoryStream>();
        contentData = snew<MemoryStream>();
        OutStream = reinterpret_pointer_cast<Stream>(contentData);
    }

    P_ContentTypeWriter ContentWriter::GetTypeWriter(Type const& type, Int& typeIndex) {
        const auto& hash = type.GetHashCode();

        if (typeTable.contains(hash)) {
            typeIndex = typeTable[hash];
            return typeWriters[typeIndex];
        }

        std::vector<P_Type> dependecies;
        auto typeWriter = compiler->GetTypeWriter(type, dependecies);
        typeIndex = typeWriters.size();

        typeWriters.push_back(typeWriter);
        typeTable.emplace(type.GetHashCode(), typeIndex);

        for (size_t i = 0; i < dependecies.size(); ++i) {
            const auto& type1 = dependecies[i];

            if (type1 != typeof<std::any>())
            {
                Int _;
                GetTypeWriter(*type1, _);
            }
        }

        return typeWriter;
    }

    void ContentWriter::WriteSharedResources() {
        while (sharedResources.size() > 0) {
            sharedResources.pop();
            auto& res = sharedResources.front();
            WriteObject<std::any>(res);
        }
    }

    void ContentWriter::WriteHeader() {
        OutStream = reinterpret_pointer_cast<Stream>(headerData);
        Write7BitEncodedInt(static_cast<Int>(typeWriters.size()));

        for (size_t i = 0; i < typeWriters.size(); ++i) {
            auto& typeWriter = typeWriters[i];

            Write(typeWriter->GetRuntimeReader(targetPlatform));
            Write(typeWriter->TypeVersion());
        }

        Write7BitEncodedInt(static_cast<Int>(sharedResourceNames.size()));
    }

    void ContentWriter::WriteFinalOutput() {
        OutStream = finalOutput;

        Write((Byte)88);
        Write((Byte)78);
        Write((Byte)66);

        if (targetPlatform == TargetPlatform::Windows)
            Write((Byte)119);
        else
            Exception::Throw(Exception::NOT_IMPLEMENTED);

        if (compressContent)
            WriteCompressedOutput();
        else
            WriteUncompressedOutput();
    }

    void ContentWriter::WriteUncompressedOutput() {
        WriteVersionNumber((Ushort)5);

        const auto length1 = static_cast<Int>(headerData->Length());
        const auto length2 = static_cast<Int>(contentData->Length());

        Write(10 + length1 + length2);

        OutStream->Write(headerData->_buffer, 0, length1);
        OutStream->Write(contentData->_buffer, 0, length2);
    }

    void ContentWriter::WriteCompressedOutput() {
        Exception::Throw(Exception::NOT_IMPLEMENTED);
    }

    void ContentWriter::WriteVersionNumber(Ushort version) {
        version |= static_cast<Ushort>(static_cast<Int>(targetProfile) << 8 & 32512);
        Write(version);
    }

    void ContentWriter::Write(Vector2 const& value) {
        Write(value.X);
        Write(value.Y);
    }

    void ContentWriter::Write(Vector3 const& value) {
        Write(value.X);
        Write(value.Y);
        Write(value.Z);
    }

    void ContentWriter::Write(Vector4 const& value) {
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

    void ContentWriter::Write(Quaternion const& value) {
        Write(value.X);
        Write(value.Y);
        Write(value.Z);
        Write(value.W);
    }

    void ContentWriter::Write(Color const& value) {
        Write(value.PackedValue());
    }

    //
    //
    //    
}