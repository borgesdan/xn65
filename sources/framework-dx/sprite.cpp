#include "xna-dx/framework.hpp"
#include <functional>

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
		std::optional<Char> const& defaultCharacter)
	{
		Exception::ThrowIfNull(texture, nameof(texture));
		Exception::ThrowIfNull(texture->impl->dxShaderResource.Get(), nameof(texture->impl->dxShaderResource));		

		if(cropping.size() != glyphs.size() || charMap.size() != glyphs.size() || (!kerning.empty() && kerning.size() != glyphs.size()))
			Exception::Throw("Cropping, charmap and kerning (if not empty) must all be the same size.");

		std::vector<DxGlyph> dxGlyps(glyphs.size());		

		for (size_t i = 0; i < dxGlyps.size(); ++i) {
			DxGlyph g;
			g.Subrect.left = static_cast<LONG>(glyphs[i].Left());
			g.Subrect.right = static_cast<LONG>(glyphs[i].Right());
			g.Subrect.top = static_cast<LONG>(glyphs[i].Top());
			g.Subrect.bottom = static_cast<LONG>(glyphs[i].Bottom());
			g.Character = static_cast<Uint>(charMap[i]);

			if (!kerning.empty()) {
				g.XOffset = kerning[i].X;
				g.YOffset = static_cast<float>(cropping[i].Y);
				g.XAdvance = kerning[i].Z + spacing;
			}
			else {
				g.XOffset = 0;
				g.YOffset = 0;
				g.XAdvance = spacing;
			}			
			
			dxGlyps[i] = g;
		}		
		
		impl = unew<PlatformImplementation>();
		impl->dxSpriteFont = unew<DxSpriteFont>(
			//ID3D11ShaderResourceView* texture
			texture->impl->dxShaderResource.Get(),
			//Glyph const* glyphs
			dxGlyps.data(),
			//size_t glyphCount
			glyphs.size(),
			//float lineSpacing
			static_cast<float>(lineSpacing)
		);

		if (defaultCharacter.has_value()) {
			const auto defChar = static_cast<wchar_t>(defaultCharacter.value());
			impl->dxSpriteFont->SetDefaultCharacter(defChar);
		}
	}	

	Vector2 SpriteFont::MeasureString(String const& text, bool ignoreWhiteSpace) {
		const auto size = impl->dxSpriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);
		Vector2 vec2{};
		vec2.X = size.m128_f32[0];
		vec2.Y = size.m128_f32[1];

		return vec2;
	}

	Vector2 SpriteFont::MeasureString(WString const& text, bool ignoreWhiteSpace){
		const auto size = impl->dxSpriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);
		Vector2 vec2{};
		vec2.X = size.m128_f32[0];
		vec2.Y = size.m128_f32[1];

		return vec2;
	}

	Char SpriteFont::DefaultCharacter() const {
		const auto defChar = impl->dxSpriteFont->GetDefaultCharacter();
		return static_cast<Char>(defChar);
	}	

	void SpriteFont::DefaultCharacter(Char value) {
		const auto defChar = static_cast<wchar_t>(value);
		impl->dxSpriteFont->SetDefaultCharacter(defChar);
	}

	Int SpriteFont::LineSpacing() const {
		const auto space = impl->dxSpriteFont->GetLineSpacing();
		return static_cast<Int>(space);
	}

	void SpriteFont::LineSpacing(float value) {
		impl->dxSpriteFont->SetLineSpacing(value);
	}		

	SpriteBatch::SpriteBatch(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		Exception::ThrowIfNull(device, nameof(device));
		Exception::ThrowIfNull(device->impl->_context.Get(), nameof(device->impl->_context));

		impl = unew<PlatformImplementation>();
		impl->dxSpriteBatch = snew<DxSpriteBatch>(
			//ID3D11DeviceContext* deviceContext
			device->impl->_context.Get()
		);

		Viewport(device->Viewport());
	}		

	void SpriteBatch::Begin(SpriteSortMode sortMode, BlendState* blendState, SamplerState* samplerState, DepthStencilState* depthStencil, RasterizerState* rasterizerState, Effect* effect, Matrix const& transformMatrix) {
		std::function<void __cdecl()> effectFunc = nullptr;

		//if Effect is not null set dxEffectBuffer and inputLayout
		if (effect) {
			bool dxEffectBufferChanged = false;

			if (!impl->dxEffectBuffer || impl->dxEffectBuffer != effect->impl->dxEffect) {
				impl->dxEffectBuffer = effect->impl->dxEffect;			
				dxEffectBufferChanged = true;
			}

			if (!impl->dxInputLayout || dxEffectBufferChanged) {
				void const* shaderByteCode;
				size_t byteCodeLength;

				effect->impl->dxEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

				m_device->impl->_device->CreateInputLayout(
					DirectX::VertexPositionColorTexture::InputElements,
					DirectX::VertexPositionColorTexture::InputElementCount,
					shaderByteCode, byteCodeLength,
					impl->dxInputLayout.GetAddressOf());
			}

			auto& context = m_device->impl->_context;

			effectFunc = [=] {
				impl->dxEffectBuffer->Apply(context.Get());
				context->IASetInputLayout(impl->dxInputLayout.Get());
				};
		}

		auto _sortMode = DxHelpers::SpriteSortToDx(sortMode);
		auto _transformMatrix = DxHelpers::MatrixToDx(transformMatrix);

		impl->dxSpriteBatch->Begin(
			_sortMode,
			blendState ? blendState->impl->dxBlendState.Get() : nullptr,
			samplerState ? samplerState->impl->_samplerState.Get() : nullptr,
			depthStencil ? depthStencil->impl->dxDepthStencil.Get() : nullptr,
			rasterizerState ? rasterizerState->impl->dxRasterizerState.Get() : nullptr,
			effectFunc,
			_transformMatrix
		);
	}

	void SpriteBatch::End() {
		impl->dxSpriteBatch->End();
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Color const& color) {
		const auto _position = DxHelpers::VectorToDx(position);
		const auto v4 = color.ToVector4();
		const auto _color = DxHelpers::VectorToDx(v4);

		impl->dxSpriteBatch->Draw(
			texture.impl->dxShaderResource.Get(),
			_position,
			_color
		);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color) {	
		const auto _position = DxHelpers::VectorToDx(position);
		const auto v4 = color.ToVector4();
		const auto _color = DxHelpers::VectorToDx(v4);

		RECT _sourceRect{};

		if (sourceRectangle.has_value()) {
			_sourceRect = DxHelpers::RectangleToDx(sourceRectangle.value());
		};

		impl->dxSpriteBatch->Draw(
			texture.impl->dxShaderResource.Get(),
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		const auto _position = DxHelpers::VectorToDx(position);
		const auto _origin = DxHelpers::VectorToDx(origin);
		const auto v4 = color.ToVector4();
		const auto _color = DxHelpers::VectorToDx(v4);

		RECT _sourceRect{};

		if (sourceRectangle.has_value()) {
			_sourceRect = DxHelpers::RectangleToDx(sourceRectangle.value());
		};

		const DxSpriteEffects _effects = static_cast<DxSpriteEffects>(effects);

		impl->dxSpriteBatch->Draw(
			texture.impl->dxShaderResource.Get(),
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			scale,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color, float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {
		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle.has_value()) {
			_sourceRect = DxHelpers::RectangleToDx(sourceRectangle.value());
		};

		const auto _effects = static_cast<DxSpriteEffects>(effects);
		const XMFLOAT2 _scale = { scale.X, scale.Y };

		impl->dxSpriteBatch->Draw(
			texture.impl->dxShaderResource.Get(),
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
		RECT _destinationRect = DxHelpers::RectangleToDx(destinationRectangle);		

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		impl->dxSpriteBatch->Draw(texture.impl->dxShaderResource.Get(), _destinationRect, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color) {
		RECT _destinationRect = DxHelpers::RectangleToDx(destinationRectangle);

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle.has_value()) {
			_sourceRect.top = sourceRectangle->Y;
			_sourceRect.left = sourceRectangle->X;
			_sourceRect.right = sourceRectangle->X + sourceRectangle->Width;
			_sourceRect.bottom = sourceRectangle->Y + sourceRectangle->Height;
		};

		impl->dxSpriteBatch->Draw(texture.impl->dxShaderResource.Get(), _destinationRect, sourceRectangle ? &_sourceRect : nullptr, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color, float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {
		RECT _destinationRect = DxHelpers::RectangleToDx(destinationRectangle);

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		RECT _sourceRect{};

		if (sourceRectangle.has_value()) {
			_sourceRect = DxHelpers::RectangleToDx(sourceRectangle.value());
		};

		auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto _effects = static_cast<DxSpriteEffects>(effects);

		impl->dxSpriteBatch->Draw(
			texture.impl->dxShaderResource.Get(),
			_destinationRect,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Viewport(xna::Viewport const& value) {
		const auto _view = DxHelpers::ViewportToDx(value);
		impl->dxSpriteBatch->SetViewport(_view);
	}

	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color) {
		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		spriteFont.impl->dxSpriteFont->DrawString(
			impl->dxSpriteBatch.get(),
			text.c_str(),
			_position,
			_color
		);
	}

	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position,
		Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };
		const auto _effects = static_cast<DxSpriteEffects>(effects);

		spriteFont.impl->dxSpriteFont->DrawString(
			impl->dxSpriteBatch.get(),
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
