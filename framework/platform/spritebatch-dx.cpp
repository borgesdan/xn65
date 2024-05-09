#include "platform-dx/blendstate-dx.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/rasterizerstate-dx.hpp"
#include "platform-dx/depthstencilstate-dx.hpp"
#include "platform-dx/samplerstate-dx.hpp"
#include "platform-dx/spritebatch-dx.hpp"
#include "platform-dx/texture-dx.hpp"

using DxSpriteBatch = DirectX::SpriteBatch;
using DxSpriteSortMode = DirectX::SpriteSortMode;
using DxMatrix = DirectX::FXMMATRIX;
using DxSpriteEffects = DirectX::SpriteEffects;
using DirectX::XMFLOAT2;
using DirectX::FXMVECTOR;
using DirectX::XMVECTORF32;
using DirectX::GXMVECTOR;

namespace xna {
	SpriteBatch::SpriteBatch(GraphicsDevice& device) {
		if (!device._context)
			return;

		_dxspriteBatch = New<DxSpriteBatch>(device._context);

		Viewport(device.Viewport());
	}
	
	void SpriteBatch::Begin(SpriteSortMode sortMode, BlendState* blendState, SamplerState* samplerState, DepthStencilState* depthStencil, RasterizerState* rasterizerState, Matrix const& transformMatrix) {

		if (!_dxspriteBatch)
			return;

		DxSpriteSortMode sort;
		ConvertSpriteSort(sortMode, sort);
		
		const auto& t = transformMatrix;
		DxMatrix matrix = DxMatrix(
			t.M11, t.M12, t.M13, t.M14,
			t.M21, t.M22, t.M23, t.M24,
			t.M31, t.M32, t.M33, t.M34,
			t.M41, t.M42, t.M43, t.M44);
		

		_dxspriteBatch->Begin(
			sort,
			blendState ? blendState->dxBlendState : nullptr,
			samplerState ? samplerState->_samplerState : nullptr,
			depthStencil ? depthStencil->dxDepthStencil : nullptr,
			rasterizerState ? rasterizerState->dxRasterizerState : nullptr,
			nullptr,
			matrix
		);
	}

	void SpriteBatch::End() {
		if (!_dxspriteBatch)
			return;

		_dxspriteBatch->End();
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Color const& color) {
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		_dxspriteBatch->Draw(
			texture.dxShaderResource,
			_position,
			_color
		);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color) {
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
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

		_dxspriteBatch->Draw(
			texture.dxShaderResource,
			_position,
			sourceRectangle ? &_sourceRect : nullptr,
			_color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Vector2 const& position, Rectangle const* sourceRectangle, Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
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

		_dxspriteBatch->Draw(
			texture.dxShaderResource,
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
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
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

		_dxspriteBatch->Draw(
			texture.dxShaderResource,
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
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
			return;

		RECT _destinationRect{};
		_destinationRect.left = destinationRectangle.X;
		_destinationRect.top = destinationRectangle.Y;
		_destinationRect.right = destinationRectangle.X + destinationRectangle.Width;
		_destinationRect.bottom = destinationRectangle.Y + destinationRectangle.Height;

		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		_dxspriteBatch->Draw(texture.dxShaderResource, _destinationRect, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color) {
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
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

		_dxspriteBatch->Draw(texture.dxShaderResource, _destinationRect, sourceRectangle ? &_sourceRect : nullptr, _color);
	}

	void SpriteBatch::Draw(Texture2D& texture, Rectangle const& destinationRectangle, Rectangle const* sourceRectangle, Color const& color, float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {
		if (!_dxspriteBatch)
			return;

		if (!texture.dxShaderResource)
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

		_dxspriteBatch->Draw(
			texture.dxShaderResource,
			_destinationRect,
			sourceRectangle ? &_sourceRect : nullptr,
			_color,
			rotation,
			_origin,
			_effects,
			layerDepth);
	}

	void SpriteBatch::Viewport(xna::Viewport const& value)	{
		if (!_dxspriteBatch)
			return;

		D3D11_VIEWPORT _view{};
		_view.TopLeftX = value.X;
		_view.TopLeftY = value.Y;
		_view.Width = value.Width;
		_view.Height = value.Height;
		_view.MinDepth = value.MinDetph;
		_view.MaxDepth = value.MaxDepth;

		_dxspriteBatch->SetViewport(_view);
	}

	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position, Color const& color) {
		if (!_dxspriteBatch || !spriteFont._dxSpriteFont)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };

		spriteFont._dxSpriteFont->DrawString(
			_dxspriteBatch.get(),
			text.c_str(),
			_position,
			_color
		);
	}
	
	void SpriteBatch::DrawString(SpriteFont& spriteFont, String const& text, Vector2 const& position,
		Color const& color, float rotation, Vector2 const& origin, float scale, SpriteEffects effects, float layerDepth) {
		if (!_dxspriteBatch || !spriteFont._dxSpriteFont)
			return;

		const auto _position = XMFLOAT2(position.X, position.Y);
		const auto _origin = XMFLOAT2(origin.X, origin.Y);
		const auto v4 = color.ToVector4();
		const XMVECTORF32 _color = { v4.X, v4.Y, v4.Z, v4.W };
		const auto _effects = static_cast<DxSpriteEffects>(effects);

		spriteFont._dxSpriteFont->DrawString(
			_dxspriteBatch.get(),
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
