#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../common/color.hpp"
#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	struct BlendRenderTarget;

	//Contains blend state for the device. 
	class BlendState : public GraphicsResource {
	public:
		BlendState();
		BlendState(sptr<GraphicsDevice> const& device);

		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		BlendFunction AlphaBlendFunction() const;
		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		void AlphaBlendFunction(BlendFunction value);
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		Blend AlphaDestinationBlend() const;
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		void AlphaDestinationBlend(Blend value);
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		Blend AlphaSourceBlend() const;
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		void AlphaSourceBlend(Blend value);

		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		BlendFunction ColorBlendFunction() const;
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		void ColorBlendFunction(BlendFunction value);
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		Blend ColorDestinationBlend() const;
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		void ColorDestinationBlend(Blend value);
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		Blend ColorSourceBlend() const;
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		void ColorSourceBlend(Blend value);

		//Gets or sets the four-component (RGBA) blend factor for alpha blending. 
		Color BlendFactor() const;
		//Gets or sets the four-component (RGBA) blend factor for alpha blending. 
		void BlendFactor(Color const& value);
		
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		Int MultiSampleMask() const;
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		void MultiSampleMast(Int value);		

		//Specifies whether to use alpha-to-coverage as a multisampling technique when setting a pixel to a render target.
		void AlphaToCoverageEnable(bool value);
		//Specifies whether to enable independent blending in simultaneous render targets
		void IndependentBlendEnable(bool value);

		void RenderTargets(std::vector<BlendRenderTarget> const& value);
		bool Initialize();
		bool Apply();

		//A built-in state object with settings for opaque blend,
		//that is overwriting the source with the destination data.
		static uptr<BlendState> Opaque();
		//A built-in state object with settings for alpha blend, 
		//that is blending the source and destination data using alpha.
		static uptr<BlendState> AlphaBlend();
		//A built-in state object with settings for additive blend, 
		//that is adding the destination data to the source data without using alpha.
		static uptr<BlendState> Additive();
		//A built-in state object with settings for blending with non-premultipled alpha, 
		//that is blending source and destination data using alpha while assuming the color data contains no alpha information.
		static uptr<BlendState> NonPremultiplied();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};	
}

#endif