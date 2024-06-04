#include "xna/content/manager.hpp"

namespace xna {
	sptr<Stream> ContentManager::OpenStream(String const& assetName) const {
		const String filePath = _rootDirectory + "\\" + assetName + contentExtension;
		const auto stream = New<FileStream>(filePath, FileMode::Open);

		if (stream->IsClosed())
			return nullptr;

		return reinterpret_pointer_cast<Stream>(stream);
	}
}