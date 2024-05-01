#ifndef XNA_CONTENT_READER_HPP
#define XNA_CONTENT_READER_HPP

#include "../default.hpp"
#include "../csharp/binary.hpp"

namespace xna {
	class ContentReader : public BinaryReader{
	public:
		static sptr<ContentReader> Create(ContentManager* contentManager, Stream* input, String const& assetName);
		
		template <typename T>
		sptr<T> ReadAsset() {
			return nullptr;
		}

	private:
		ContentReader(ContentManager* contentManager, sptr<Stream>const& input, String const& assetName)
			: BinaryReader(input), _contentManager(contentManager), _assetName(assetName){}

		static sptr<Stream> PrepareStream(sptr<Stream>& input, String const* assetName, Int& graphicsProfile);

		Int ReadHeader();

	private:
		ContentManager* _contentManager = nullptr;
		String _assetName;
	};
}

#endif