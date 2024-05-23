#ifndef XNA_PLATFORM_CONTENTREADERS_TEXTURE2D_HPP
#define XNA_PLATFORM_CONTENTREADERS_TEXTURE2D_HPP

#include "../../content/manager.hpp"
#include "../../content/reader.hpp"
#include "../../csharp/type.hpp"
#include "graphics/texture.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	class Texture2DReader : public ContentTypeReaderT<PTexture2D> {
	public:
		Texture2DReader() : ContentTypeReaderT(typeof<Texture2D>()){}

		PTexture2D Read(ContentReader& input, PTexture2D& existingInstance) override{
			const auto format = static_cast<SurfaceFormat>(input.ReadInt32());
			const auto width = input.ReadInt32();
			const auto height = input.ReadInt32();
			const auto mipMaps = input.ReadInt32();	

			auto a_device =  ContentManager::Services()->GetService(*typeof<GraphicsDevice>());
			sptr<GraphicsDevice> device = nullptr;

			if(a_device.has_value())
				device = std::any_cast<sptr<GraphicsDevice>>(a_device);

			auto texture2D = New<Texture2D>(device, width, height, mipMaps, format);

			for (size_t level = 0; level < mipMaps; ++level) {
				auto elementCount = input.ReadInt32();
				std::vector<Byte> data = input.ReadByteBuffer(elementCount);

				texture2D->SetData(static_cast<Int>(level), nullptr, data, 0, elementCount);
			}			

			return texture2D;
		}
	};
}

#endif