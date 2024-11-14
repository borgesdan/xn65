#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "../default.hpp"
#include "presentparams.hpp"
#include "viewport.hpp"

namespace xna {
	//Performs primitive-based rendering, creates resources, handles system-level variables, adjusts gamma ramp levels, and creates shaders. 
	class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice> {
	public:
		GraphicsDevice();
		
		GraphicsDevice(P_GraphicsAdapter const& adapter, GraphicsProfile const& graphicsProfile, P_PresentationParameters const& presentationParameters);

		//Gets the graphics adapter.
		inline P_GraphicsAdapter Adapter() const { return adapter; }
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		inline P_BlendState BlendState() const { return blendState; }
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		void BlendState(P_BlendState const& value);
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		inline P_DepthStencilState DepthStencilState() const { return depthStencilState; }
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		void DepthStencilState(P_DepthStencilState const& value);
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		inline P_RasterizerState RasterizerState() const { return rasterizerState; }
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		void RasterizerState(P_RasterizerState const& value);
		//Retrieves a collection of SamplerState objects for the current GraphicsDevice. 
		inline P_SamplerStateCollection SamplerStates() const { return samplerStateCollection; }
		//Gets the graphics profile.
		constexpr GraphicsProfile Profile() const { return graphicsProfile; }
		//Gets the presentation parameters associated with this graphics device.
		P_PresentationParameters PresentParameters() const { return presentationParameters; }
		//Clears resource buffers. 
		void Clear(Color const& color) const;
		//Clears resource buffers. 
		void Clear(ClearOptions options, Color const& color, float depth, Int stencil) const;		
		//Presents the display with the contents of the next buffer in the sequence of back buffers owned by the GraphicsDevice.
		bool Present() const;		
		//Resets the presentation parameters for the current GraphicsDevice.
		void Reset(P_PresentationParameters const& presentationParameters, P_GraphicsAdapter const& graphicsAdapter);
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		constexpr xna::Viewport Viewport() const { return viewport; }
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		void Viewport(xna::Viewport const& viewport);
		//Sets a new render target for this GraphicsDevice. 
		void SetRenderTarget(P_RenderTarget2D const& renderTarget) { this->renderTarget = renderTarget; }

		void Initialize();

	private:
		P_GraphicsAdapter adapter{ nullptr };
		P_BlendState blendState{ nullptr };
		P_DepthStencilState depthStencilState{ nullptr };
		P_RasterizerState rasterizerState{ nullptr };
		P_SamplerStateCollection samplerStateCollection{ nullptr };
		P_PresentationParameters presentationParameters{ nullptr };
		P_RenderTarget2D renderTarget{ nullptr };
		GraphicsProfile graphicsProfile{ GraphicsProfile::HiDef };
		xna::Viewport viewport{};

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif