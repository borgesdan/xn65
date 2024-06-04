#include "xna/graphics/rasterizerstate.hpp"
#include "xna/graphics/samplerstate.hpp"
#include "xna/common/color.hpp"
#include "xna/common/numerics.hpp"
#include "xna/graphics/sprite.hpp"
#include "xna/graphics/viewport.hpp"
#include "xna/graphics/blendstate.hpp"
#include "xna/graphics/depthstencilstate.hpp"
#include "xna/platform-dx/implementations.hpp"
#include <stdexcept>

using DxSpriteBatch = DirectX::SpriteBatch;
using DxSpriteSortMode = DirectX::SpriteSortMode;
using DxMatrix = DirectX::FXMMATRIX;
using DxSpriteEffects = DirectX::SpriteEffects;
using DirectX::XMFLOAT2;
using DirectX::FXMVECTOR;
using DirectX::XMVECTORF32;
using DirectX::GXMVECTOR;
using DxSpriteFont = DirectX::SpriteFont;
using DxGlyph = DirectX::SpriteFont::Glyph;

namespace xna {
	SpriteFont::SpriteFont(
		sptr<Texture2D> const& texture,
		std::vector<Rectangle> const& glyphs,
		std::vector<Rectangle> const& cropping,
		std::vector<Char> const& charMap,
		Int lineSpacing,
		float spacing,
		std::vector<Vector3> const& kerning,
		std::optional<Char> defaultCharacter) :
		textureValue(texture), glyphData(glyphs), croppingData(cropping),
		characterMap(charMap), lineSpacing(lineSpacing), spacing(spacing),
		kerning(kerning), defaultCharacter(defaultCharacter)
	{
		if (!texture)
			throw std::invalid_argument("SpriteFont: texture is null.");

		std::vector<DxGlyph> dxGlyps(glyphs.size());		

		for (size_t i = 0; i < dxGlyps.size(); ++i) {
			DxGlyph g;
			g.Subrect.left = glyphs[i].Left();
			g.Subrect.right = glyphs[i].Right();
			g.Subrect.top = glyphs[i].Top();
			g.Subrect.bottom = glyphs[i].Bottom();
			g.Character = static_cast<Uint>(charMap[i]);
			g.XOffset = kerning[i].X;
			g.YOffset = cropping[i].Y;
			g.XAdvance = kerning[i].Z;
			dxGlyps[i] = g;
		}		
		
		impl = uNew<PlatformImplementation>();
		impl->_dxSpriteFont = unew<DxSpriteFont>(
			//ID3D11ShaderResourceView* texture
			texture->impl->dxShaderResource,
			//Glyph const* glyphs
			dxGlyps.data(),
			//size_t glyphCount
			glyphs.size(),
			//float lineSpacing
			static_cast<float>(lineSpacing)
		);

		if (defaultCharacter.has_value()) {
			const auto defChar = static_cast<wchar_t>(defaultCharacter.value());
			impl->_dxSpriteFont->SetDefaultCharacter(defChar);
		}
		else {
			impl->_dxSpriteFont->SetDefaultCharacter(charMap[0]);
		}
	}

	SpriteFont::~SpriteFont() {
		impl = nullptr;
	}

	Vector2 SpriteFont::MeasureString(String const& text, bool ignoreWhiteSpace)
	{
		if (!impl->_dxSpriteFont)
			return Vector2();

		const auto size = impl->_dxSpriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);
		Vector2 vec2{};
		vec2.X = size.m128_f32[0];
		vec2.Y = size.m128_f32[1];

		return vec2;
	}

	Vector2 SpriteFont::MeasureString(WString const& text, bool ignoreWhiteSpace)
	{
		if (!impl->_dxSpriteFont)
			return Vector2();

		const auto size = impl->_dxSpriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);
		Vector2 vec2{};
		vec2.X = size.m128_f32[0];
		vec2.Y = size.m128_f32[1];

		return vec2;
	}

	static constexpr void ConvertSpriteSort(SpriteSortMode value, DirectX::SpriteSortMode& target) {
		target = static_cast<DirectX::SpriteSortMode>(static_cast<int>(value));
	}

	SpriteBatch::SpriteBatch(GraphicsDevice& device) {
		if (!device.impl->_context)
			return;

		implementation = uNew<PlatformImplementation>();
		implementation->_dxspriteBatch = New<DxSpriteBatch>(device.impl->_context);

		Viewport(device.Viewport());
	}

	SpriteBatch::~SpriteBatch() {
	}

	void SpriteBatch::Begin(SpriteSortMode sortMode, BlendState* blendState, SamplerState* samplerState, DepthStencilState* depthStencil, RasterizerState* rasterizerState, Matrix const& transformMatrix) {

		if (!implementation->_dxspriteBatch)
			return;

		DxSpriteSortMode sort;
		ConvertSpriteSort(sortMode, sort);

		const auto& t = transformMatrix;
		DxMatrix matrix = DxMatrix(
			t.M11, t.M12, t.M13, t.M14,
			t.M21, t.M22, t.M23, t.M24,
			t.M31, t.M32, t.M33, t.M34,
			t.M41, t.M42, t.M43, t.M44);


		implementation->_dxspriteBatch->Begin(
			sort,
			blendState ? blendState->impl->dxBlendState : nullptr,
			samplerState ? samplerState->impl->_samplerState : nullptr,
			depthStencil ? depthStencil->impl->dxDepthStencil : nullptr,
			rasterizerState ? rasterizerState->impl->dxRasterizerState : nullptr,
			nullptr,
			matrix
		);
	}

	void SpriteBatch::End() {
		if (!implementation->_dxspriteBatch)
			return;

		implementation->_dxspriteBatch->End();
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Color const& color) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		implementation->_dxspriteBatch->Draw(
			texture.impl->dxShaderResource,
			_position,
			_color
		);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		implementation->_dxspriteBatch->Draw(
			texture.impl->dxShaderResource,
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		const DxSpriteEffects _effects = static_cast<DxSpriteEffects>(effects);

		implementation->_dxspriteBatch->Draw(
			texture.impl->dxShaderResource,
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			scale,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color, float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		const auto _effects = static_cast<DxSpriteEffects>(effects);
		const XMFLOAT2 _scale = { scale.X, scale.Y };

		implementation->_dxspriteBatch->Draw(
			texture.impl->dxShaderResource,
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			_scale,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, Color const& color) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		RECT _destinationRect{};
		_destinationRect.left = destinationRectangle.X;
		_destinationRect.top = destinationRectangle.Y;
		_destinationRect.right = destinationRectangle.X + destinationRectangle.Width;
		_destinationRect.bottom = destinationRectangle.Y + destinationRectangle.Height;

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		implementation->_dxspriteBatch->Draw(texture.impl->dxShaderResource, _destinationRect, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		RECT _destinationRect{};
		_destinationRect.left = destinationRectangle.X;
		_destinationRect.top = destinationRectangle.Y;
		_destinationRect.right = destinationRectangle.X + destinationRectangle.Width;
		_destinationRect.bottom = destinationRectangle.Y + destinationRectangle.Height;

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		implementation->_dxspriteBatch->Draw(texture.impl->dxShaderResource, _destinationRect, sourceRectangle ? &_sourceRect : nullptr, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color, float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {
		if (!implementation->_dxspriteBatch)
			return;

		if (!texture.impl->dxShaderResource)
			return;

		RECT _destinationRect{};
		_destinationRect.left = destinationRectangle.X;
		_destinationRect.top = destinationRectangle.Y;
		_destinationRect.right = destinationRectangle.X + destinationRectangle.Width;
		_destinationRect.bottom = destinationRectangle.Y + destinationRectangle.Height;

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto _effects = static_cast<DxSpriteEffects>(effects);

		implementation->_dxspriteBatch->Draw(
			texture.impl->dxShaderResource,
			_destinationRect,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Viewport(xna::Viewport const& value) {
		if (!implementation->_dxspriteBatch)
			return;

		D3D11_VIEWPORT _view{};
		_view.TopLeftX = value.X;
		_view.TopLeftY = value.Y;
		_view.Width = value.Width;
		_view.Height = value.Height;
		_view.MinDepth = value.MinDetph;
		_view.MaxDepth = value.MaxDepth;

		implementation->_dxspriteBatch->SetViewport(_view);
	}

	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color) {
		if (!implementation->_dxspriteBatch || !spriteFont.impl->_dxSpriteFont)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		spriteFont.impl->_dxSpriteFont->DrawString(
			implementation->_dxspriteBatch.get(),
			text.c_str(),
			_position,
			_color
		);
	}

	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position,
		Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		if (!implementation->_dxspriteBatch || !spriteFont.impl->_dxSpriteFont)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };
		const auto _effects = static_cast<DxSpriteEffects>(effects);

		spriteFont.impl->_dxSpriteFont->DrawString(
			implementation->_dxspriteBatch.get(),
			text.c_str(),
			_position,
			_color,
			rotation,
			_origin,
			scale,
			_effects,
			layerDepth
		);
	}
}
