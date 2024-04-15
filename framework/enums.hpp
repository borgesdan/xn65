#ifndef XNA_ENUMS_HPP
#define XNA_ENUMS_HPP

namespace xna {
	enum class Blend {
		Zero,
		One,
		SourceColor,
		InverseSourceColor,
		SourceAlpha,
		InverseSourceAlpha,
		DestinationAlpha,
		InverseDestinationAlpha,
		DestinationColor,
		InverseDestinationColor,
		SourceAlphaSaturation,
		BlendFactor,
		InverseBlendFactor,
		Source1Color,
		InverseSource1Color,
		Source1Alpha,
		InverseSource1Alpha
	};	

	enum class BlendFunction {
		Add = 0,
		Subtract = 1,
		ReverseSubtract = 2,
		Min = 3,
		Max = 4,
	};

	using BlendOperation = BlendFunction;

	enum class ColorWriteChannels {
		Red,
		Green,
		Blue,
		Alpha,
		All
	};

	enum class ComparisonFunction {
		Never,
		Less,
		Equal,
		LessEquals,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	enum class CullMode {
		None,
		CullClockwiseFace,
		CullCounterClockwiseFace,
	};

	enum class DepthFormat {
		None,
		Depth16,
		Depth24,
		Depth24Stencil8
	};

	enum class DepthWriteMask {
		Zero,
		All
	};

	enum class DisplayOrientation {
		Default = 0,
		LandscapeLeft = 1,
		LandscapeRight = 2,
		Portrait = 4,
	};

	enum class FillMode
	{
		Solid,
		WireFrame,
	};

	enum class GraphicsProfile {
		Reach,
		HiDef
	};

	enum class PresentInterval {
		Default,
		One,
		Two,
		Immediate
	};

	enum RenderTargetUsage {
		DiscardContents,
		PreserveContents,
		PlatformContents
	};

	enum class SeekOrigin {
		Begin,
		Current,
		End,
	};

	enum class SpriteEffects {
		None = 0,
		FlipHorizontally = 1,
		FlipVertically = 2,
		Both = FlipHorizontally | FlipVertically
	};

	enum class SpriteSortMode
	{
		Deferred,
		Immediate,
		Texture,
		BackToFront,
		FrontToBack,
	};

	enum class StencilOperation
	{
		Keep,
		Zero,
		Replace,
		IncrementSaturation,
		DecrementSaturation,
		Invert,
		Increment,
		Decrement,
	};

	enum class SurfaceFormat {
		Color = 0,
		Bgr565 = 1,
		Bgra5551 = 2,
		Bgra4444 = 3,
		Dxt1 = 4,
		Dxt3 = 5,
		Dxt5 = 6,
		NormalizedByte2 = 7,
		NormalizedByte4 = 8,
		Rgba1010102 = 9,
		Rg32 = 10,
		Rgba64 = 11,
		Alpha8 = 12,
		Single = 13,
		Vector2 = 14,
		Vector4 = 15,
		HalfSingle = 16,
		HalfVector2 = 17,
		HalfVector4 = 18,
		HdrBlendable = 19,
	};

	enum class TextureAddressMode {
		Wrap,
		Clamp,
		Mirror,
		Border,
		MirrorOnce
	};

	enum class TextureFilter {
		Linear,
		Point,
		Anisotropic,
		LinearMipPoint,
		PointMipLinear,
		MinLinearMagPointMipLinear,
		MinLinearMagPointMipPoint,
		MinPointMagLinearMipLinear,
		MinPointMagLinearMipPoint,
	};



	constexpr int SURFACE_FORMAT_COUNT = 19;
}

#endif