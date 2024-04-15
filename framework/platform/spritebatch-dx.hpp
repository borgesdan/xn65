#ifndef XNA_PLATFORM_SPRITEBATCH_DX_HPP
#define XNA_PLATFORM_SPRITEBATCH_DX_HPP

#include "../common/color.hpp"
#include "../common/rectangle.hpp"
#include "../common/vectors.hpp"
#include "../graphics/spritebatch.hpp"
#include "../graphics/viewport.hpp"
#include "SpriteBatch.h"
#include "spritefont-dx.hpp"

namespace xna {
	class SpriteBatch : public ISpriteBatch {
	public:
		SpriteBatch(GraphicsDevice& device);

		virtual ~SpriteBatch() override {}

		virtual void Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred,
			BlendState* blendState = nullptr,
			SamplerState* samplerState = nullptr,
			DepthStencilState* depthStencil = nullptr,
			RasterizerState* rasterizerState = nullptr,
			//Effect
			Matrix const& transformMatrix = _identity
		) override;

		virtual void End() override;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Color const& color) override;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const * sourceRectangle, Color const& color) override;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) override;
		virtual void Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) override;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Color const& color) override;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color) override;
		virtual void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) override;
		virtual void Viewport(xna::Viewport const& value) override;	
		virtual void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color) override;
		virtual void DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) override;

	public:
		static constexpr void ConvertSpriteSort(SpriteSortMode value, DirectX::SpriteSortMode& target) {
			switch (value)
			{
			case xna::SpriteSortMode::Deferred:
				target = DirectX::SpriteSortMode_Deferred;
				break;
			case xna::SpriteSortMode::Immediate:
				target = DirectX::SpriteSortMode_Immediate;
				break;
			case xna::SpriteSortMode::Texture:
				target = DirectX::SpriteSortMode_Texture;
				break;
			case xna::SpriteSortMode::BackToFront:
				target = DirectX::SpriteSortMode_BackToFront;
				break;
			case xna::SpriteSortMode::FrontToBack:
				target = DirectX::SpriteSortMode_FrontToBack;
				break;
			default:
				target = DirectX::SpriteSortMode_Deferred;
				break;
			}
		}

	public:
		sptr<DirectX::SpriteBatch> _dxspriteBatch = nullptr;		
	};
}

#endif