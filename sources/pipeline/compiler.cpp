#include "pipeline/compiler.hpp"
#include "pipeline/writer.hpp"

namespace xna {
	ContentCompiler::ContentCompiler() {
		const auto writers = typeWriterFactory->Initialize();

		for (auto& writer : writers)
			AddTypeWriter(writer);

		for (auto& dic : writerInstances) {
			auto& writer = dic.second;
			AddTypeWriter(writer);
		}
	}

	void ContentCompiler::Compile(
		P_Stream const& output,
		Object& value,
		TargetPlatform targetPlatform,
		GraphicsProfile targetProfile,
		bool compressContent,
		String const& rootDirectory,
		String const& referenceRelocationPath) {
		if (compressContent)
			compressContent = ShouldCompressContent(targetPlatform, value);

		auto _this = shared_from_this();

		auto contentWriter = unew<ContentWriter>(
			_this, output, targetPlatform,
			targetProfile, compressContent,
			rootDirectory, referenceRelocationPath);

		contentWriter->WriteObject(value);
		contentWriter->FlushOutput();
	}
}