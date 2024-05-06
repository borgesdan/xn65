#ifndef XNA_PLATFORM_CONTENTREADERS_TEXTURE2D_HPP
#define XNA_PLATFORM_CONTENTREADERS_TEXTURE2D_HPP

#include "../../content/reader.hpp"
#include "../texture-dx.hpp"

namespace xna {
	class Texture2DReader : public ContentTypeReaderT<Texture2D> {
	public:
		Texture2DReader() : ContentTypeReaderT(typeof<Texture2D>()){}

		Texture2D Read(ContentReader& input, Texture2D& existingInstance) override{
			const auto format = static_cast<SurfaceFormat>(input.ReadInt32());
			const auto width = input.ReadInt32();
			const auto height = input.ReadInt32();
			const auto mipMaps = input.ReadInt32();	
			auto texture2D = Texture2D(nullptr, width, height, mipMaps, format);

			for (size_t level = 0; level < mipMaps; ++level) {
				auto elementCount = input.ReadInt32();
				std::vector<Byte> data = input.ReadByteBuffer(elementCount);

				texture2D.SetData(level, nullptr, data, 0, elementCount);
			}			

			return texture2D;
		}
	};
}

#endif