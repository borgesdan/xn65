#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "presentparams.hpp"
#include "viewport.hpp"
#include <memory>

namespace xna {
	struct GraphicsDeviceImplementation;

	//Performs primitive-based rendering, creates resources, handles system-level variables, adjusts gamma ramp levels, and creates shaders. 
	class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice> {
	public:
		GraphicsDevice();

		GraphicsDevice(
			std::shared_ptr<GraphicsAdapter> const& adapter,
			GraphicsProfile const& graphicsProfile,
			std::shared_ptr<PresentationParameters> const& presentationParameters);

		//Gets the graphics adapter.
		inline std::shared_ptr<GraphicsAdapter> Adapter() const { return adapter; }
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		inline std::shared_ptr<xna::BlendState> BlendState() const { return blendState; }
		//Gets or sets a system-defined instance of a blend state object initialized for alpha blending. The default value is BlendState.Opaque. 
		void BlendState(std::shared_ptr<xna::BlendState> const& value);
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		inline std::shared_ptr<xna::DepthStencilState> DepthStencilState() const { return depthStencilState; }
		//Gets or sets a system-defined instance of a depth-stencil state object. The default value is DepthStencilState.Default.
		void DepthStencilState(std::shared_ptr<xna::DepthStencilState> const& value);
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		inline std::shared_ptr<xna::RasterizerState> RasterizerState() const { return rasterizerState; }
		//Gets or sets rasterizer state. The default value is RasterizerState.CullCounterClockwise.
		void RasterizerState(std::shared_ptr<xna::RasterizerState> const& value);
		//Retrieves a collection of SamplerState objects for the current GraphicsDevice. 
		inline std::shared_ptr<SamplerStateCollection> SamplerStates() const { return samplerStateCollection; }
		//Gets the graphics profile.
		constexpr GraphicsProfile Profile() const { return graphicsProfile; }
		//Gets the presentation parameters associated with this graphics device.
		std::shared_ptr<PresentationParameters> PresentParameters() const { return presentationParameters; }
		//Clears resource buffers. 
		void Clear(Color const& color) const;
		//Clears resource buffers. 
		void Clear(ClearOptions options, Color const& color, float depth, Int stencil) const;
		//Presents the display with the contents of the next buffer in the sequence of back buffers owned by the GraphicsDevice.
		bool Present() const;
		//Resets the presentation parameters for the current GraphicsDevice.
		void Reset(std::shared_ptr<PresentationParameters> const& presentationParameters, std::shared_ptr<GraphicsAdapter> const& graphicsAdapter);
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		constexpr xna::Viewport Viewport() const { return viewport; }
		//Gets or sets a viewport identifying the portion of the render target to receive draw calls. 
		void Viewport(xna::Viewport const& viewport);
		//Sets a new render target for this GraphicsDevice. 
		void SetRenderTarget(std::shared_ptr<RenderTarget2D> const& renderTarget) { this->renderTarget = renderTarget; }

		void Initialize();

		std::unique_ptr<GraphicsDeviceImplementation> Implementation;

	private:
		std::shared_ptr<GraphicsAdapter> adapter{ nullptr };
		std::shared_ptr<xna::BlendState> blendState{ nullptr };
		std::shared_ptr<xna::DepthStencilState> depthStencilState{ nullptr };
		std::shared_ptr<xna::RasterizerState> rasterizerState{ nullptr };
		std::shared_ptr<SamplerStateCollection> samplerStateCollection{ nullptr };
		std::shared_ptr<PresentationParameters> presentationParameters{ nullptr };
		std::shared_ptr<RenderTarget2D> renderTarget{ nullptr };
		GraphicsProfile graphicsProfile{ GraphicsProfile::HiDef };
		xna::Viewport viewport{};
	};
}

#endif