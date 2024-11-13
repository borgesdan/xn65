#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../common/color.hpp"
#include "../platform.hpp"
#include "gresource.hpp"
#include <cstdint>
#include <vector>

namespace xna {
	//Defines color blending factors.
	enum class Blend {
		//Each component of the color is multiplied by (0, 0, 0, 0). 
		Zero,
		//Each component of the color is multiplied by (1, 1, 1, 1)
		One,
		//Each component of the color is multiplied by the source color. This can be represented as (Rs, Gs, Bs, As), 
		// where R, G, B, and A respectively stand for the red, green, blue, and alpha source values. 
		SourceColor,
		//Each component of the color is multiplied by the inverse of the source color. 
		//This can be represented as (1 − Rs, 1 − Gs, 1 − Bs, 1 − As) where R, G, B, and A respectively stand for the red, green, blue, and alpha destination values. 
		InverseSourceColor,
		//Each component of the color is multiplied by the alpha value of the source. 
		//This can be represented as (As, As, As, As), where As is the alpha source value. 
		SourceAlpha,
		//Each component of the color is multiplied by the inverse of the alpha value of the source. 
		//This can be represented as (1 − As, 1 − As, 1 − As, 1 − As), where As is the alpha destination value. 
		InverseSourceAlpha,
		//Each component of the color is multiplied by the alpha value of the destination.
		//This can be represented as (Ad, Ad, Ad, Ad), where Ad is the destination alpha value. 
		DestinationAlpha,
		//Each component of the color is multiplied by the inverse of the destination color.
		//This can be represented as (1 − Rd, 1 − Gd, 1 − Bd, 1 − Ad), where Rd, Gd, Bd, and Ad respectively stand for the red, green, blue, and alpha destination values.
		InverseDestinationAlpha,
		//Each component color is multiplied by the destination color.
		//This can be represented as (Rd, Gd, Bd, Ad), where R, G, B, and A respectively stand for red, green, blue, and alpha destination values. 
		DestinationColor,
		//Each component of the color is multiplied by the inverse of the destination color.
		// This can be represented as (1 − Rd, 1 − Gd, 1 − Bd, 1 − Ad), where Rd, Gd, Bd, and Ad respectively stand for the red, green, blue, and alpha destination values. 
		InverseDestinationColor,
		//Each component of the color is multiplied by either the alpha of the source color, or the inverse of the alpha of the source color, whichever is greater.
		// This can be represented as (f, f, f, 1), where f = min(A, 1 − Ad). 
		SourceAlphaSaturation,
		//Each component of the color is multiplied by a constant set in BlendFactor. 
		BlendFactor,
		//Each component of the color is multiplied by the inverse of a constant set in BlendFactor. 
		InverseBlendFactor,
		Source1Color,
		InverseSource1Color,
		Source1Alpha,
		InverseSource1Alpha
	};

	//Defines how to combine a source color with the destination color already on the render target for color blending.
	enum class BlendFunction {
		//The result is the destination added to the source.
		//Result = (Source Color * Source Blend) + (Destination Color * Destination Blend)
		Add = 0,
		//The result is the destination subtracted from the source.
		//Result = (Source Color * Source Blend) −(Destination Color * Destination Blend)
		Subtract = 1,
		//The result is the source subtracted from the destination.
		//Result = (Destination Color * Destination Blend) −(Source Color * Source Blend)
		ReverseSubtract = 2,
		//The result is the minimum of the source and destination.
		//Result = min((Source Color * Source Blend), (Destination Color * Destination Blend))
		Min = 3,
		//The result is the maximum of the source and destination.
		//Result = max((Source Color * Source Blend), (Destination Color * Destination Blend))
		Max = 4,
	};

	//Defines the color channels that can be chosen for a per-channel write to a render target color buffer. 
	enum class ColorWriteChannels {
		//Red channel of a buffer.
		Red,
		//Green channel of a buffer.
		Green,
		//Blue channel of a buffer.
		Blue,
		//Alpha channel of a buffer.
		Alpha,
		//All buffer channels.
		All,
		//No channel selected.
		None
	};

	using BlendOperation = BlendFunction;

	struct BlendRenderTarget {
		bool Enabled{ true };
		Blend Source{ Blend::SourceAlpha };
		Blend Destination{ Blend::InverseSourceAlpha };
		BlendOperation Operation{ BlendOperation::Add };
		Blend SourceAlpha{ Blend::One };
		Blend DestinationAlpha{ Blend::Zero };
		BlendOperation OperationAlpha{ BlendOperation::Add };
		ColorWriteChannels WriteMask{ ColorWriteChannels::All };

		constexpr BlendRenderTarget() = default;
	};

	struct BlendStateImplementation;

	//Contains blend state for the device. 
	class BlendState : public GraphicsResource, public ImplementationBase<BlendStateImplementation> {
	public:
		BlendState();
		BlendState(std::shared_ptr<GraphicsDevice> const& device);

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
		int32_t MultiSampleMask() const;
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		void MultiSampleMask(int32_t value);		

		//Specifies whether to use alpha-to-coverage as a multisampling technique when setting a pixel to a render target.
		void AlphaToCoverageEnable(bool value);
		//Specifies whether to enable independent blending in simultaneous render targets
		void IndependentBlendEnable(bool value);

		void RenderTargets(std::vector<BlendRenderTarget> const& value) { return RenderTargets(value.data(), value.size()); }
		void RenderTargets(BlendRenderTarget const* value, size_t size);

		bool Initialize();
		bool Apply();

		//A built-in state object with settings for opaque blend,
		//that is overwriting the source with the destination data.
		static std::unique_ptr<BlendState> Opaque();
		//A built-in state object with settings for alpha blend, 
		//that is blending the source and destination data using alpha.
		static std::unique_ptr<BlendState> AlphaBlend();
		//A built-in state object with settings for additive blend, 
		//that is adding the destination data to the source data without using alpha.
		static std::unique_ptr<BlendState> Additive();
		//A built-in state object with settings for blending with non-premultipled alpha, 
		//that is blending source and destination data using alpha while assuming the color data contains no alpha information.
		static std::unique_ptr<BlendState> NonPremultiplied();
	};	

	using PBlendState = std::shared_ptr<BlendState>;
}

#endif