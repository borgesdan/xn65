#ifndef XNA_GRAPHICS_SPRITE_HPP
#define XNA_GRAPHICS_SPRITE_HPP

#include "../common/color.hpp"
#include "../common/numerics.hpp"
#include "../graphics/gresource.hpp"
#include <memory>
#include <optional>
#include <string>

namespace xna {
	//Defines sprite sort-rendering options.
	enum class SpriteSortMode
	{
		//Sprites are not drawn until End is called.
		//End will apply graphics device settings and draw all the sprites in one batch, in the same order calls to Draw were received.
		//This mode allows Draw calls to two or more instances of SpriteBatch without introducing conflicting graphics device settings.
		//SpriteBatch defaults to Deferred mode.
		Deferred,
		//Begin will apply new graphics device settings, and sprites will be drawn within each Draw call.
		//In Immediate mode there can only be one active SpriteBatch instance without introducing conflicting device settings. 
		Immediate,
		//Same as Deferred mode, except sprites are sorted by texture prior to drawing.
		//This can improve performance when drawing non-overlapping sprites of uniform depth.
		Texture,
		//Same as Deferred mode, except sprites are sorted by depth in back-to-front order prior to drawing.
		//This procedure is recommended when drawing transparent sprites of varying depths.
		BackToFront,
		//Same as Deferred mode, except sprites are sorted by depth in front-to-back order prior to drawing.
		//This procedure is recommended when drawing opaque sprites of varying depths.
		FrontToBack,
	};

	struct SpriteBatchImplementation;

	//Enables a group of sprites to be drawn using the same settings. 
	class SpriteBatch : public GraphicsResource  {
	public:
		SpriteBatch(std::shared_ptr<GraphicsDevice> const& device);

		//Begins a sprite batch operation. 		
		void Begin(
			std::optional<SpriteSortMode> sortMode,
			std::unique_ptr<BlendState> blendState,
			std::unique_ptr<SamplerState> samplerState,
			std::unique_ptr<DepthStencilState> depthStencil,
			std::unique_ptr<RasterizerState> rasterizerState,
			std::unique_ptr<Effect> effect,
			Matrix const& transformMatrix = Matrix::Identity()) 
		{
			Begin(
				sortMode.has_value() ? sortMode.value() : SpriteSortMode::Deferred,
				blendState.get(),
				samplerState.get(),
				depthStencil.get(),
				rasterizerState.get(),
				effect.get(),
				transformMatrix);
		}

		//Begins a sprite batch operation. 
		void Begin(
			std::optional<SpriteSortMode> sortMode,
			std::shared_ptr<BlendState> blendState,
			std::shared_ptr<SamplerState> samplerState,
			std::shared_ptr<DepthStencilState> depthStencil,
			std::shared_ptr<RasterizerState> rasterizerState,
			std::shared_ptr<Effect> effect,
			Matrix const& transformMatrix = Matrix::Identity()) {
			Begin(
				sortMode.has_value() ? sortMode.value() : SpriteSortMode::Deferred,
				blendState.get(),
				samplerState.get(),
				depthStencil.get(),
				rasterizerState.get(),
				effect.get(),
				transformMatrix);
		}

		//Begins a sprite batch operation. 
		void Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred,
			BlendState* blendState = nullptr,
			SamplerState* samplerState = nullptr,
			DepthStencilState* depthStencil = nullptr,
			RasterizerState* rasterizerState = nullptr,
			Effect* effect = nullptr,
			Matrix const& transformMatrix = Matrix::Identity()
		);

		//Flushes the sprite batch and restores the device state to how it was before Begin was called.
		void End();
		
		//
		// Draw - Adds a sprite to a batch of sprites to be rendered.
		//

		void Draw(std::unique_ptr<Texture2D> const& texture, Vector2 const& position, Color const& color) { Draw(*texture, position, color); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Vector2 const& position, Color const& color) { Draw(*texture, position, color); }
		void Draw(Texture2D& texture, Vector2 const& position, Color const& color);

		void Draw(std::unique_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color) { Draw(*texture, position, sourceRectangle, color); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color) { Draw(*texture, position, sourceRectangle, color); }
		void Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color);

		void Draw(std::unique_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) { Draw(*texture, position, sourceRectangle, color, rotation, origin, scale, effects, layerDepth); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) { Draw(*texture, position, sourceRectangle, color, rotation, origin, scale, effects, layerDepth); }
		void Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth);

		void Draw(std::unique_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {	Draw(*texture, position, sourceRectangle, color, rotation, origin, scale, effects, layerDepth); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {	Draw(*texture, position, sourceRectangle, color, rotation, origin, scale, effects, layerDepth); }
		void Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth);

		void Draw(std::unique_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, Color const& color) {	Draw(*texture, destinationRectangle, color); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, Color const& color) {	Draw(*texture, destinationRectangle, color); }
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, Color const& color);

		void Draw(std::unique_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color) { Draw(*texture, destinationRectangle, sourceRectangle, color); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color) { Draw(*texture, destinationRectangle, sourceRectangle, color); }
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color);

		void Draw(std::unique_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) { Draw(*texture, destinationRectangle, sourceRectangle, color, rotation, origin, effects, layerDepth); }
		void Draw(std::shared_ptr<Texture2D> const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) { Draw(*texture, destinationRectangle, sourceRectangle, color, rotation, origin, effects, layerDepth); }
		void Draw(Texture2D& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth);		

		//
		// DrawString - Adds a string to a batch of sprites to be rendered.
		//

		void DrawString(std::unique_ptr<SpriteFont> const& spriteFont, std::string const& text, Vector2 const& position, Color const& color) { DrawString(*spriteFont, text, position, color); }
		void DrawString(std::shared_ptr<SpriteFont> const& spriteFont, std::string const& text, Vector2 const& position, Color const& color) { DrawString(*spriteFont, text, position, color); }
		void DrawString(SpriteFont& spriteFont, std::string const& text, Vector2 const& position, Color const& color);

		void DrawString(std::unique_ptr<SpriteFont> const& spriteFont, std::string const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) { DrawString(*spriteFont, text, position, color, rotation, origin, scale, effects, layerDepth); }
		void DrawString(std::shared_ptr<SpriteFont> const& spriteFont, std::string const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) { DrawString(*spriteFont, text, position, color, rotation, origin, scale, effects, layerDepth); }
		void DrawString(SpriteFont& spriteFont, std::string const& text, Vector2 const& position, Color const& color,
			float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth);

		std::unique_ptr<SpriteBatchImplementation> Implementation;
	};

	struct SpriteFontImplementation;

	//Represents a font texture.
	class SpriteFont {
	public:
		SpriteFont(
			std::shared_ptr<Texture2D> const& texture,
			std::vector<Rectangle> const& glyphs,
			std::vector<Rectangle> const& cropping,
			std::vector<Char> const& charMap,
			int32_t lineSpacing,
			float spacing,
			std::vector<Vector3> const& kerning,
			std::optional<Char> const& defaultCharacter);
		
		// Returns the width and height of a string.
		Vector2 MeasureString(std::string const& text, bool ignoreWhiteSpace = true);
		// Returns the width and height of a string.
		Vector2 MeasureString(std::wstring const& text, bool ignoreWhiteSpace = true);	

		//Gets or sets the default character for the font.
		Char DefaultCharacter() const;
		//Gets or sets the default character for the font.
		void DefaultCharacter(Char value);
		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		int32_t LineSpacing() const;
		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		void LineSpacing(float value);

		std::unique_ptr<SpriteFontImplementation> Implementation;
	};
}

#endif