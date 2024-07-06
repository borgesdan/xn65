#ifndef XNA_PIPELINE_COMPILER_HPP
#define XNA_PIPELINE_COMPILER_HPP

#include "../common/color.hpp"
#include "../common/numerics.hpp"
#include "../csharp/binary.hpp"
#include "../csharp/type.hpp"
#include "../default.hpp"
#include "pipeline.hpp"
#include <any>
#include <map>
#include <queue>

namespace xna {
	class ContentTypeWriter;

	class ContentCompiler {

	};

	class ContentWriter : public BinaryWriter {
	public:
		ContentWriter(
			sptr<ContentCompiler> const& compiler,
			sptr<Stream> const& output,
			TargetPlatform targetPlatform,
			GraphicsProfile targetProfile,
			bool compressContent,
			String const& rootDirectory,
			String const& referenceRelocationPath)
			: compiler(compiler), finalOutput(output), targetPlatform(targetPlatform),
			targetProfile(targetProfile), compressContent(compressContent), rootDirectory(rootDirectory),
			referenceRelocationPath(referenceRelocationPath) {

			headerData = snew<MemoryStream>();
			contentData = snew<MemoryStream>();
			_stream = reinterpret_pointer_cast<Stream>(contentData);
		}

		void FlushOutput();

		template <typename T>
		void WriteObject(T& value);

		template <typename T>
		void WriteObject(T& value, ContentTypeWriter& typeWriter);

		template <typename T>
		void WriteRawObject(T& value);

		template <typename T>
		void WriteRawObject(T& value, ContentTypeWriter& typeWriter);

		template <typename T>
		void WriteSharedResource(T* value) {
			if (!value) {
				Write7BitEncodedInt(0);
			}
			else {
				Exception::Throw(ExMessage::NotImplemented);
			}
		}

		template <typename T>
		void WriteSharedResource(T& value, ContentTypeWriter& typeWriter);

		template <typename T>
		void WriteSharedResource(ExternalReference<T>* reference) {
			if (!value) {
				Write(std::string());
			}
			else {
				const String filename1 = reference->Filename();

				if (filename1.empty())
					Write(std::string());
				else {
					const String filename2;

					if (filename1.ends_with(".xnb"))
						filename2 = filename1.substr(0, filename1.length() - 4);
					else
						Exception::Throw(ExMessage::ArgumentFormat);

					//.Write(PathUtils.GetRelativePath(PathUtils.GetAbsoluteUri(this.referenceRelocationPath), filename2));
				}
			}

		}

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

	private:
		template <typename T>
		void InvokeWriter(T& value, ContentTypeWriter& typeWriter);

		sptr<ContentTypeWriter> GetTypeWriter(Type& type, size_t& typeIndex);
		void WriteSharedResources() {
			Exception::Throw(ExMessage::NotImplemented);
		}

		void WriteHeader();
		void WriteFinalOutput();
		void WriteUncompressedOutput();

		void WriteCompressedOutput() {
			Exception::Throw(ExMessage::NotImplemented);
		}

		void WriteVersionNumber(Ushort version);		

	private:
		static constexpr Ushort XnbVersion = 5;
		static constexpr Ushort XnbCompressedVersion = 32773;
		static constexpr Ushort XnbVersionProfileMask = 32512;
		static constexpr Int XnbVersionProfileShift = 8;
		static constexpr Int XnbVersionOffset = 4;
		static constexpr Int XnbFileSizeOffset = 6;
		static constexpr Int XnbPrologueSize = 10;

		sptr<ContentCompiler> const& compiler;
		bool compressContent;
		String rootDirectory;
		String referenceRelocationPath;
		TargetPlatform targetPlatform;
		GraphicsProfile targetProfile;

		sptr<Stream> finalOutput;
		sptr<MemoryStream> headerData;
		sptr<MemoryStream> contentData;

		std::map<size_t, size_t> typeTable;
		std::vector<sptr<ContentTypeWriter>> typeWriters;
		std::map<object, bool> recurseDetector;
		std::map<object, Int> sharedResourceNames;
		std::queue<object> sharedResources;
	};

	class ContentTypeWriter {
	public:
		virtual String GetRuntimeReader(TargetPlatform targetPlatform) = 0;
		virtual Int TypeVersion() { return 0; }

	protected:
		ContentTypeWriter(sptr<Type> const& targetType) : targetType(targetType) {
			targetIsValueType = targetType->IsValueType();
		}

	private:
		sptr<Type> targetType = nullptr;
		bool targetIsValueType{ false };
	};

	template <typename T>
	class ContentTypeWriterT : public ContentTypeWriter {
	protected:
		ContentTypeWriterT() : ContentTypeWriter(typeof<T>()) {
		}
	};

	template <typename T>
	class BuiltinTypeWriter : public ContentTypeWriter<T> {
	public:

	private:

	};
}

#endif