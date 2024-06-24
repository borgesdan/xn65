#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "../default.hpp"

namespace xna {
	//Performs primitive-based rendering, creates resources, handles system-level variables, adjusts gamma ramp levels, and creates shaders. 
	class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice> {
	public:
		GraphicsDevice();
		GraphicsDevice(GraphicsDeviceInformation const& info);

		//Gets the graphics adapter.
		sptr<GraphicsAdapter> Adapter() const;

		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		sptr<xna::BlendState> BlendState() const;
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		void BlendState(sptr<xna::BlendState> const& value);
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		sptr<xna::DepthStencilState> DepthStencilState() const;
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		void DepthStencilState(sptr<xna::DepthStencilState> const& value);
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		sptr<xna::RasterizerState> RasterizerState() const;
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		void RasterizerState(sptr<xna::RasterizerState> const& value);	
		//Retrieves a collection of SamplerState objects for the current GraphicsDevice. 
		sptr<SamplerStateCollection> SamplerStates() const;

		//Gets or sets a bitmask controlling modification of the samples in a multisample render target. The default value is -1 (0xffffffff). 
		Int MultiSampleMask() const;
		//Gets or sets a bitmask controlling modification of the samples in a multisample render target. The default value is -1 (0xffffffff). 
		void MultiSampleMask(Int value);

		void Clear();
		void Clear(Color const& color);
		bool Initialize();
		bool Present();

		xna::Viewport Viewport() const;
		void Viewport(xna::Viewport const& viewport);
		void UseVSync(bool use);

		//void DrawPrimitives(PrimitiveType primitiveType, Int startVertex, Int primitiveCount);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	using PGraphicsDevice = sptr<GraphicsDevice>;
}

#endif