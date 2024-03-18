#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "texture.hpp"


namespace xna {
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D(GraphicsDevice* device);

		PLATFORM_DEVELOPMENT
			bool Apply();

	private:
		GraphicsDevice* _device;

	public:
		class InternalProperty;
		friend class InternalProperty;
		sptr<InternalProperty> ip_RenderTarget2D{ nullptr };
	};
}

#endif