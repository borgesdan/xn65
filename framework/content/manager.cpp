#include "xna/content/manager.hpp"

namespace xna {
	sptr<Stream> ContentManager::OpenStream(String const& assetName) const {
		const String filePath = rootDirectory + "\\" + assetName + contentExtension;
		const auto stream = snew<FileStream>(filePath, FileMode::Open);		

		return reinterpret_pointer_cast<Stream>(stream);
	}
}