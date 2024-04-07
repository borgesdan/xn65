#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../forward.hpp"
#include "../types.hpp"
#include "../enums.hpp"

namespace xna {
	class Texture {
	};

	class ITexture2D : public Texture {
	public:
		virtual ~ITexture2D(){}
		virtual Int Width() const = 0;
		virtual Int Height() const = 0;
	};
}

#endif