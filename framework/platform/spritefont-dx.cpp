#include "platform-dx/spritefont-dx.hpp"

using DxSpriteFont = DirectX::SpriteFont;

namespace xna {
	SpriteFont::SpriteFont(GraphicsDevice& device, String const& fontFileName)
	{
		const auto wString = XnaHToWString(fontFileName);
		_dxSpriteFont = New<DxSpriteFont>(device._device, wString.c_str());
	}

	Vector2 SpriteFont::MeasureString(String const& text, bool ignoreWhiteSpace)
	{
		if (!_dxSpriteFont)
			return Vector2();

		const auto size = _dxSpriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);
		Vector2 vec2{};
		vec2.X = size.m128_f32[0];
		vec2.Y = size.m128_f32[1];

		return vec2;
	}
}