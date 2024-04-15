#ifndef XNA_GRAPHICS_SPRITEFONT_HPP
#define XNA_GRAPHICS_SPRITEFONT_HPP

#include "../default.hpp"

namespace xna {
	class ISpriteFont {
	public:
		virtual ~ISpriteFont(){}

		virtual Vector2 MeasureString(String const& text, bool ignoreWhiteSpace = true) = 0;
	};
}

#endif