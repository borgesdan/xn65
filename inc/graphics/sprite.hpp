#ifndef XNA_GRAPHICS_SPRITE_HPP
#define XNA_GRAPHICS_SPRITE_HPP

#include "../default.hpp"
#include "common/numerics.hpp"
#include "common/color.hpp"

namespace xna {
	class SpriteBatch {
	public:
		SpriteBatch(GraphicsDevice& device);
		~SpriteBatch();
		void Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred,
			BlendState* blendState = nullptr,
			SamplerState* samplerState = nullptr,
			DepthStencilState* depthStencil = nullptr,
			RasterizerState* rasterizerState = nullptr,
			//Effect
			Matrix const& transformMatrix = Matrix::Identity()
		);
		void End();
		void Draw(Texture2D& texture, Vector2 const& position, Color const& color);
		void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color);
		void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth);
		void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth);
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Color const& color);
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color);
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth);
		void Viewport(xna::Viewport const& value);
		void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color);
		void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> implementation = nullptr;
	};

	class SpriteFont {
	public:
		SpriteFont(GraphicsDevice& device, String const& fontFileName);
		~SpriteFont();
		Vector2 MeasureString(String const& text, bool ignoreWhiteSpace = true);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> implementation = nullptr;
	};
}

#endif