#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../forward.hpp"
#include "../types.hpp"
#include "../enums.hpp"

namespace xna {
	class Texture {
	};

	class Texture2D : public Texture {
	public:
		PLATFORM_DEVELOPMENT
			Texture2D();

	public:
		class InternalProperty;
		friend class InternalProperty;
		sptr<InternalProperty> ip_Texture2D{ nullptr };
	};
}

#endif