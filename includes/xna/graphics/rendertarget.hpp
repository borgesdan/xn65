#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "texture.hpp"

namespace xna {
	struct RenderTarget2DImplementation;

	//Contains a 2D texture that can be used as a render target. 
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D();
		RenderTarget2D(std::shared_ptr<GraphicsDevice> const& device);				

		static P_RenderTarget2D FromBackBuffer(std::shared_ptr<GraphicsDevice> const& device);

		void Initialize();	
		
		std::unique_ptr<RenderTarget2DImplementation> Implementation2;
	};	
}

#endif