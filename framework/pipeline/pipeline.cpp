#include "xna/pipeline/pipeline.hpp"

namespace xna {
	String ContentBuilderLogger::GetCurrentFilename(sptr<ContentIdentity> const& contentIdentity) {
		String str;

		if (contentIdentity && !contentIdentity->SourceFilename.empty())
			GetRelativeFilename(contentIdentity->SourceFilename, LoggerRootDirectory, str);

		if (fileStack.size() > 0)
			GetRelativeFilename(fileStack.top(), LoggerRootDirectory, str);

		return str;
	}

	void ContentBuilderLogger::GetRelativeFilename(String const& filename, String const& rootDirectory, String& out)
	{
		if (rootDirectory.empty())
			out = std::filesystem::current_path().string();

		out = filename.starts_with(rootDirectory)
			? filename.substr(0, rootDirectory.length())
			: filename;
	}
}