#include "reader.hpp"
#include "manager.hpp"

namespace xna {
	sptr<ContentReader> ContentReader::Create(ContentManager* contentManager, Stream& input, String const& assetName)
	{
		return sptr<ContentReader>();
	}
}
