#ifndef XNA_PIPELINE_WRITER_HPP
#define XNA_PIPELINE_WRITER_HPP

#include "xna/common/color.hpp"
#include "xna/common/numerics.hpp"
#include "xna/csharp/binary.hpp"
#include "xna/csharp/type.hpp"
#include "xna/default.hpp"
#include "default.hpp"
#include "enums.hpp"
#include <queue>

namespace xna {
    //Provides an implementation for many of the ContentCompiler methods including compilation, state tracking for shared resources and creation of the header type manifest.
    class ContentTypeWriter {
    public:
        //Gets the assembly qualified name of the runtime loader for this type.
        virtual String GetRuntimeReader(TargetPlatform targetPlatform) { return String(); }
        //Gets a format version number for this type.
        virtual Int TypeVersion() const { return 0; }
        //Compiles an object into binary format.
        virtual void Write(ContentWriter& output, Object& value) {}
        //Determines if deserialization into an existing object is possible.
        virtual bool CanDeserializeIntoExistingObject() const { return false; }

    protected:
        ContentTypeWriter(P_Type const& targetType) : targetType(targetType) {}

    private:
        P_Type targetType{ nullptr };
    };

    //Provides a generic implementation of ContentTypeWriter methods and properties for compiling a specific managed type into a binary format.
    template <class T>
    class ContentTypeWriter_T : public ContentTypeWriter {
    public:
        //Compiles a strongly typed object into binary format.
        virtual void Write(ContentWriter& output, T& value) {}

    protected:
        ContentTypeWriter_T() : ContentTypeWriter(typeof<T>()) {}
    };

    template <class T>
    class BuiltinTypeWriter : public ContentTypeWriter_T<T> {
        String GetRuntimeReader(TargetPlatform targetPlatform) override {
            String name = typeid(T).name();

            switch (targetPlatform)
            {
            case xna::TargetPlatform::Windows:
                name.append("::Windows");
                break;
            default:
                break;
            }

            return name;
        }
    };

    template <class T>
    class ExternalReference {
    public:
        String FileName() const;
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
        P_ContentTypeWriter GetTypeWriter(Type const& type, Int& typeIndex);
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
        std::map<Object, Int> sharedResourceNames;
        std::queue<Object> sharedResources;
        std::map<HashValue, Int> typeTable;

    private:
        static constexpr Ushort XnbVersion = 5;
        static constexpr Ushort XnbCompressedVersion = 32773;
        static constexpr Ushort XnbVersionProfileMask = 32512;
        static constexpr Int XnbVersionProfileShift = 8;
        static constexpr Int XnbVersionOffset = 4;
        static constexpr Int XnbFileSizeOffset = 6;
        static constexpr Int XnbPrologueSize = 10;
        inline static const String FilenameExt = ".xnb";
    };

    template <class T>
    class TypeHandlerFactory {

    };

    class ContentTypeWriterFactory : public TypeHandlerFactory<ContentTypeWriter> {
    public:
        std::vector<P_ContentTypeWriter> Initialize() const {
            std::vector<P_ContentTypeWriter> writers;

            return writers;
        }
    };

    //
    // ContentTypeWriter
    //

    template <class T>
    void ContentWriter::WriteObject(T& value) {
        if constexpr (XnaHelper::IsSmartPoint<T>()) {
            if (value == nullptr) {
                Exception::Throw(Exception::ARGUMENT_IS_NULL);
            }
        }

        const auto type = typeof<T>();
        Int _;
        auto typeWriter = GetTypeWriter(*type, _);

        InvokeWriter<T>(value, *typeWriter);
    }

    template <class T>
    void ContentWriter::WriteObject(T& value, ContentTypeWriter& writer) {
        auto contentTypeWriter = reinterpret_cast<ContentTypeWriter_T<T>*>(&writer);

        if (contentTypeWriter) {
            contentTypeWriter->Write(*this, value);
        }
        else {
            Object _value = value;
            writer.Write(*this, _value);
        }
    }

    template <class T>
    void ContentWriter::WriteRawObject(T& value) {
        if constexpr (XnaHelper::IsSmartPoint<T>()) {
            if (value == nullptr) {
                Exception::Throw(Exception::ARGUMENT_IS_NULL);
            }
        }

        const auto type = typeof<T>();
        Int _;
        auto typeWriter = GetTypeWriter(*type, _);

        InvokeWriter<T>(value, *typeWriter);
    }

    template <class T>
    void ContentWriter::WriteRawObject(T& value, ContentTypeWriter& typeWriter) {
        Exception::ThrowTIsNull(value);

        InvokeWriter<T>(value, typeWriter);
    }


    template <class T>
    void ContentWriter::WriteSharedResource(T& value) {
        if constexpr (XnaHelper::IsSmartPoint<T>()) {
            if (value == nullptr)
                Write7BitEncodedInt(0);
        }
        else {
            Int num;
            Object obj = value;

            if (!sharedResourceNames.contains(value)) {
                num = sharedResourceNames.size() + 1;
                sharedResourceNames.emplace(obj, num);
                sharedResources.push(obj);
            }
            else {
                num = sharedResourceNames[value];
            }

            Write7BitEncodedInt(num);
        }
    }


    template <class T>
    void ContentWriter::WriteExternalReference(ExternalReference<T>& reference) {
        const auto& filename1 = reference.FileName();

        if (filename1.empty()) {
            Write("");
        }
        else {
            String filename2;

            if (filename1.ends_with(FilenameExt))
                filename2 = filename1.substr(0, filename1.size() - FilenameExt.size());
            else
                Exception::Throw(Exception::INVALID_OPERATION);

            if (!filename2.starts_with(rootDirectory))
                Exception::Throw(Exception::INVALID_OPERATION);


            Exception::Throw(Exception::NOT_IMPLEMENTED);
        }
    }

    template <class T>
    void ContentWriter::InvokeWriter(T& value, ContentTypeWriter& writer) {
        auto contentTypeWriter = reinterpret_cast<ContentTypeWriter_T<T>*>(&writer);

        if (contentTypeWriter) {
            contentTypeWriter->Write(*this, value);
        }
        else {
            Object obj = value;
            writer.Write(*this, obj);
        }
    }
}

#endif