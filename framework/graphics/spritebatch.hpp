#ifndef XNA_GRAPHICS_SPRITEBATCH_HPP
#define XNA_GRAPHICS_SPRITEBATCH_HPP

#include "../default.hpp"
#include "../common/matrix.hpp"

namespace xna {
	class ISpriteBatch {
	public:
		static constexpr Matrix _identity = Matrix::Identity();

		virtual ~ISpriteBatch(){}
		virtual void Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred,
			BlendState* blendState = nullptr,
			SamplerState* samplerState = nullptr,
			DepthStencilState * depthStencil = nullptr,
			RasterizerState* rasterizerState = nullptr,
		//Effect
			Matrix const& transformMatrix = _identity
		) = 0;
		virtual void End() = 0;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Color const& color) = 0;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const * sourceRectangle, Color const& color) = 0;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color, 
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) = 0;	
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) = 0;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Color const& color) = 0;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color) = 0;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) = 0;
		virtual void Viewport(xna::Viewport const& value) = 0;
		virtual void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color) = 0;
		virtual void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) = 0;
	};
}

#endif