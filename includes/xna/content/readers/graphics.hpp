#ifndef XNA_CONTENT_READERS_GRAPHICS_HPP
#define XNA_CONTENT_READERS_GRAPHICS_HPP

#include "../../common/numerics.hpp"
#include "../../csharp/timespan.hpp"
#include "../../csharp/type.hpp"
#include "../../graphics/sprite.hpp"
#include "../../graphics/texture.hpp"
#include "../manager.hpp"
#include "../reader.hpp"

namespace xna {
	class Texture2DReader : public ContentTypeReaderT<PTexture2D> {
	public:
		Texture2DReader() : ContentTypeReaderT(typeof<PTexture2D>()) {
			ContentTypeReader::TargetIsValueType = false;
		}

		PTexture2D Read(ContentReader& input, PTexture2D& existingInstance) override {
			const auto format = static_cast<SurfaceFormat>(input.ReadInt32());
			const auto width = input.ReadInt32();
			const auto height = input.ReadInt32();
			const auto mipMaps = input.ReadInt32();

			auto a_device = ContentManager::GameServiceProvider()->GetService(*typeof<GraphicsDevice>());
			sptr<GraphicsDevice> device = nullptr;

			if (a_device.has_value())
				device = std::any_cast<sptr<GraphicsDevice>>(a_device);

			auto texture2D = snew<Texture2D>(device, width, height, mipMaps, format);

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
		SpriteFontReader() : ContentTypeReaderT(typeof<PSpriteFont>()) {
			ContentTypeReader::TargetIsValueType = false;
		}

		PSpriteFont Read(ContentReader& input, PSpriteFont& existingInstance) override {
			const auto texture = input.ReadObject<PTexture2D>();
			const auto glyphs = input.ReadObject<std::vector<Rectangle>>();
			const auto cropping = input.ReadObject<std::vector<Rectangle>>();
			const auto charMap = input.ReadObject<std::vector<Char>>();
			const auto lineSpacing = input.ReadInt32();
			const auto spacing = input.ReadSingle();
			const auto kerning = input.ReadObject<std::vector<Vector3>>();
			std::optional<Char> defaultCharacter;

			if (input.ReadBoolean())
				defaultCharacter = std::optional<Char>(input.ReadChar());

			auto font = snew<SpriteFont>(texture, glyphs, cropping, charMap, lineSpacing, spacing, kerning, defaultCharacter);
			return font;
		}
	};
}

#endif