#ifndef XNA_PIPELINE_COMPILER_HPP
#define XNA_PIPELINE_COMPILER_HPP

#include "xna/default.hpp"
#include "xna/csharp/stream.hpp"
#include "pipeline-enums.hpp"
#include "default.hpp"
#include <stack>

namespace xna {
	//Provides methods for writing compiled binary format.
	class ContentCompiler : public std::enable_shared_from_this<ContentCompiler> {
	public:
		ContentCompiler();

		//Retrieves the worker writer for the specified type
		P_ContentTypeWriter GetTypeWriter(Type const& type, std::vector<P_Type> dependencies);

	private:
		void Compile(
			P_Stream const& output,
			Object& value,
			TargetPlatform targetPlatform,
			GraphicsProfile targetProfile,
			bool compressContent,
			String const& rootDirectory,
			String const& referenceRelocationPath);

		void AddTypeWriter(P_ContentTypeWriter const& writer);

		bool ShouldCompressContent(TargetPlatform targetPlatform, Object& value) {
			return false;
		}

	private:
		using TypeList = std::vector<P_Type>;

		std::map<HashValue, P_ContentTypeWriter> writerInstances;
		std::map<HashValue, TypeList> writerDependecies;
		std::stack<P_ContentTypeWriter> initializeContext;
		P_ContentTypeWriterFactory typeWriterFactory;
	};
}

#endif