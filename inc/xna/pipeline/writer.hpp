#ifndef XNA_PIPELINE_WRITER_HPP
#define XNA_PIPELINE_WRITER_HPP

#include "../csharp/binary.hpp"
#include "../default.hpp"
#include "pipeline-enums.hpp"
#include "../common/numerics.hpp"
#include "../common/color.hpp"

namespace xna {
    class ContentTypeWriter {
    public:
        virtual String GetRuntimeReader(TargetPlatform targetPlatform) = 0;
        virtual Int TypeVersion() const { return 0; }
    };

    template <class T>
    class ExternalReference {

    };

    //Provides an implementation for many of the ContentCompiler methods including compilation, state tracking for shared resources and creation of the header type manifest.
	class ContentWriter : public BinaryWriter {
    public:
        ContentWriter(
            P_ContentCompiler& compiler,
            P_Stream const& output,
            TargetPlatform targetPlatform,
            GraphicsProfile targetProfile,
            bool compressContent,
            String const& rootDirectory,
            String const& referenceRelocationPath
        );

        //Gets the content build target platform.
        constexpr TargetPlatform Target() const { return targetPlatform; }
        //Gets or sets the target graphics profile.
        constexpr GraphicsProfile TargetProfile() const { return targetProfile; }

        //Writes a single object preceded by a type identifier to the output binary.
        template <class T> void WriteObject(T& value);
        //Writes a single object to the output binary, using the specified type hint and writer worker.
        template <class T> void WriteObject(T& value, ContentTypeWriter& writer);
        //Writes a single object to the output binary as an instance of the specified type.
        template <class T> void WriteRawObject(T& value);
        //Writes a single object to the output binary using the specified writer worker.
        template <class T> void WriteRawObject(T& value, ContentTypeWriter& typeWriter);
        //Adds a shared reference to the output binary and records the object to be serialized later.
        template <class T> void WriteSharedResource(T& value);
        //Writes the name of an external file to the output binary.
        template <class T> void WriteExternalReference(ExternalReference<T>& reference);

        using BinaryWriter::Write;

        //Writes a Vector2 value.
        void Write(Vector2 const& value);
        //Writes a Vector3 value.
        void Write(Vector3 const& value);
        //Writes a Vector4 value.
        void Write(Vector4 const& value);
        //Writes a Matrix value.
        void Write(Matrix const& value);
        //Writes a Quaternion value.
        void Write(Quaternion const& value);
        //Writes a Color value.
        void Write(Color const& value);

        inline void FlushOutput() {
            WriteSharedResources();
            WriteHeader();
            WriteFinalOutput();
        }

    private:
        template <class T> void InvokeWriter(T& value, ContentTypeWriter& writer);
        sptr<ContentTypeWriter> GetTypeWriter(Type const& type, int& typeIndex) { return nullptr; }
        void WriteSharedResources();
        void WriteHeader();
        void WriteFinalOutput();
        void WriteUncompressedOutput();        
        void WriteCompressedOutput();
        void WriteVersionNumber(Ushort version);

    private:
        P_ContentCompiler compiler{ nullptr };
        TargetPlatform targetPlatform{ TargetPlatform::Windows };
        GraphicsProfile targetProfile{ GraphicsProfile::HiDef };
        bool compressContent{ false };
        String rootDirectory;
        String referenceRelocationPath;
        P_Stream finalOutput{ nullptr };
        P_MemoryStream headerData{ nullptr };
        P_MemoryStream contentData{ nullptr };
        std::vector<P_ContentTypeWriter> typeWriters;
        std::map<std::any, Int> sharedResourceNames;
        std::queue<std::any> sharedResources;

    private:
        static constexpr Ushort XnbVersion = 5;
        static constexpr Ushort XnbCompressedVersion = 32773;
        static constexpr Ushort XnbVersionProfileMask = 32512;
        static constexpr Int XnbVersionProfileShift = 8;
        static constexpr Int XnbVersionOffset = 4;
        static constexpr Int XnbFileSizeOffset = 6;
        static constexpr Int XnbPrologueSize = 10;
	};

    //
    // Generics implementations
    //

    template <class T>
    void ContentWriter::WriteObject(T& value) {

    }

    template <class T> 
    void ContentWriter::WriteObject(T& value, ContentTypeWriter& writer) {

    }
        
    template <class T> 
    void ContentWriter::WriteRawObject(T& value) {

    }
    
    template <class T> 
    void ContentWriter::WriteRawObject(T& value, ContentTypeWriter& typeWriter) {

    }

    
    template <class T> 
    void ContentWriter::WriteSharedResource(T& value) {

    }

    
    template <class T> 
    void ContentWriter::WriteExternalReference(ExternalReference<T>& reference) {

    }

    template <class T> 
    void ContentWriter::InvokeWriter(T& value, ContentTypeWriter& writer) {

    }
}

#endif