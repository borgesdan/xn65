#ifndef XNA_CONTENT_READERS_GRAPHICS_HPP
#define XNA_CONTENT_READERS_GRAPHICS_HPP

#include "content/manager.hpp"
#include "content/reader.hpp"
#include "csharp/type.hpp"
#include "graphics/texture.hpp"
#include "common/numerics.hpp"
#include "csharp/timespan.hpp"
#include "graphics/sprite.hpp"

namespace xna {
	class Texture2DReader : public ContentTypeReaderT<PTexture2D> {
	public:
		Texture2DReader() : ContentTypeReaderT(typeof<PTexture2D>()) {}

		PTexture2D Read(ContentReader& input, PTexture2D& existingInstance) override {
			const auto format = static_cast<SurfaceFormat>(input.ReadInt32());
			const auto width = input.ReadInt32();
			const auto height = input.ReadInt32();
			const auto mipMaps = input.ReadInt32();

			auto a_device = ContentManager::Services()->GetService(*typeof<GraphicsDevice>());
			sptr<GraphicsDevice> device = nullptr;

			if (a_device.has_value())
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

	class SpriteFontReader : public ContentTypeReaderT<PSpriteFont> {
	public:
		SpriteFontReader() : ContentTypeReaderT(typeof<PSpriteFont>()) {}

		PSpriteFont Read(ContentReader& input, PSpriteFont& existingInstance) override {
			auto texture = input.ReadObject<PTexture2D>();
			auto glyphs = input.ReadObject<std::vector<Rectangle>>();
			auto cropping = input.ReadObject<std::vector<Rectangle>>();
			auto charMap = input.ReadObject<std::vector<char>>();
			auto lineSpacing = input.ReadInt32();
			auto spacing = input.ReadSingle();
			auto kerning = input.ReadObject<std::vector<Vector3>>();
			std::optional<Char> defaultCharacter;

			if (input.ReadBoolean())
				defaultCharacter = std::optional<Char>(input.ReadChar());

			auto font = snew<SpriteFont>(texture, glyphs, cropping, charMap, lineSpacing, spacing, kerning, defaultCharacter);
			return font;
		}
	};
}

#endif