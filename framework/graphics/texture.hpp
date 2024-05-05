#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../default.hpp"

namespace xna {
	class Texture {
	};

	class ITexture2D : public Texture {
	public:
		virtual ~ITexture2D(){}
		virtual Int Width() const = 0;
		virtual Int Height() const = 0;
		virtual Rectangle Bounds() const = 0;
		virtual bool Initialize(xna_error_nullarg) = 0;
	};
}

#endif