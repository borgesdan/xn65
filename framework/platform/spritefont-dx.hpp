#ifndef XNA_PLATFORM_SPRITEFONT_DX_HPP
#define XNA_PLATFORM_SPRITEFONT_DX_HPP

#include "../graphics/spritefont.hpp"
#include "../common/vectors.hpp"
#include "SpriteFont.h"
#include "device-dx.hpp"

namespace xna {
	class SpriteFont : public ISpriteFont {
	public:
		SpriteFont(GraphicsDevice& device, String const& fontFileName);

		virtual ~SpriteFont() override {}
		
		Vector2 MeasureString(String const& text, bool ignoreWhiteSpace = true) override;

	public:
		sptr<DirectX::SpriteFont> _dxSpriteFont = nullptr;
	};
}

#endif