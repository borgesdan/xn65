#include "xna/content/manager.hpp"

namespace xna {
	std::shared_ptr<csharp::Stream> ContentManager::OpenStream(std::string const& assetName) const {
		const auto filePath = rootDirectory + "\\" + assetName + contentExtension;
		const auto stream = snew<csharp::FileStream>(filePath, csharp::FileMode::Open);		

		return reinterpret_pointer_cast<csharp::Stream>(stream);
	}
}