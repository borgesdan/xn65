#ifndef XNA_CONTENT_READER_HPP
#define XNA_CONTENT_READER_HPP

#include "../default.hpp"

namespace xna {
	class ContentReader {
	public:
		static sptr<ContentReader> Create(ContentManager* contentManager, Stream& input, String const& assetName);

	private:
		//ContentReader(ContentManager* contentManager, Stream& inut)
	};
}

#endif