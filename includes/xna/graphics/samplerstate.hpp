#ifndef XNA_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_GRAPHICS_SAMPLERSTATE_HPP

#include "gresource.hpp"
#include "shared.hpp"
#include <memory>
#include <vector>

namespace xna {
	//Defines modes for addressing texels using texture coordinates that are outside of the typical range of 0.0 to 1.0.
	enum class TextureAddressMode {
		//Tile the texture at every integer junction.
		//For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed. 
		Wrap,
		//Similar to Wrap, except that the texture is flipped at every integer junction. 
		//For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again, and so on.
		Mirror,
		//Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
		Clamp,
		//Texture coordinates outside the range [0.0, 1.0] are set to the border color specified.
		Border,
		//Similar to Mirror and Clamp.
		//Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
		MirrorOnce
	};

	//Defines filtering types during texture sampling. 
	enum class TextureFilter {
		//Use linear filtering.
		Linear,
		//Use point filtering.
		Point,
		//Use anisotropic filtering.
		Anisotropic,
		//Use linear filtering to shrink or expand, and point filtering between mipmap levels (mip).
		LinearMipPoint,
		//Use point filtering to shrink (minify) or expand (magnify), and linear filtering between mipmap levels.
		PointMipLinear,
		//Use linear filtering to shrink, point filtering to expand, and linear filtering between mipmap levels.
		MinLinearMagPointMipLinear,
		//Use linear filtering to shrink, point filtering to expand, and point filtering between mipmap levels.
		MinLinearMagPointMipPoint,
		//Use point filtering to shrink, linear filtering to expand, and linear filtering between mipmap levels.
		MinPointMagLinearMipLinear,
		//Use point filtering to shrink, linear filtering to expand, and point filtering between mipmap levels.
		MinPointMagLinearMipPoint,
	};

	struct SamplerStateImplementation;

	//Contains sampler state, which determines how to sample texture data. 
	class SamplerState : public GraphicsResource {
	public:
		SamplerState();
		SamplerState(std::shared_ptr<GraphicsDevice> const& device);
		
		//Gets or sets the maximum anisotropy. The default value is 0.
		void MaxAnisotropy(uint32_t value);
		//Gets or sets the maximum anisotropy. The default value is 0.
		uint32_t MaxAnisotropy() const;
		//Gets or sets the type of filtering during sampling.
		void Filter(TextureFilter value);
		//Gets or sets the type of filtering during sampling.
		TextureFilter Filter() const;
		////Gets or sets the texture-address mode for the u-coordinate.
		void AddressU(TextureAddressMode value);
		//Gets or sets the texture-address mode for the u-coordinate.
		TextureAddressMode AddressU() const;
		//Gets or sets the texture-address mode for the v-coordinate.
		TextureAddressMode AddressV() const;
		//Gets or sets the texture-address mode for the v-coordinate.
		void AddressV(TextureAddressMode value);
		//Gets or sets the texture-address mode for the w-coordinate.
		TextureAddressMode AddressW() const;
		////Gets or sets the texture-address mode for the w-coordinate.
		void AddressW(TextureAddressMode value);		

		//Gets or sets the mipmap LOD bias. The default value is 0. 
		void MipMapLevelOfDetailBias(float value);
		//Gets or sets the mipmap LOD bias. The default value is 0. 
		float MipMapLevelOfDetailBias() const;

		//Gets or sets the level of detail (LOD) index of the largest map to use. 
		float MaxMipLevel() const;
		//Gets or sets the level of detail (LOD) index of the largest map to use. 
		void MaxMipLevel(float value);

		//Gets or sets the level of detail (LOD) index of the smaller map to use. 
		void MinMipLevel(float value);
		//Gets or sets the level of detail (LOD) index of the smaller map to use. 
		float MinMipLevel() const;		

		//Contains default state for point filtering and texture coordinate wrapping.
		static std::unique_ptr<SamplerState> PoinWrap();
		//Contains default state for point filtering and texture coordinate clamping.
		static std::unique_ptr<SamplerState> PointClamp();
		//Contains default state for linear filtering and texture coordinate wrapping.
		static std::unique_ptr<SamplerState> LinearWrap();
		//Contains default state for linear filtering and texture coordinate clamping.
		static std::unique_ptr<SamplerState> LinearClamp();
		//Contains default state for anisotropic filtering and texture coordinate wrapping.
		static std::unique_ptr<SamplerState> AnisotropicWrap();
		//Contains default state for anisotropic filtering and texture coordinate clamping.
		static std::unique_ptr<SamplerState> AnisotropicClamp();

		ComparisonFunction Comparison() const;
		void Comparison(ComparisonFunction value);

		bool Initialize();
		bool Apply();	

		std::unique_ptr<SamplerStateImplementation> Implementation;
	};	

	//Collection of SamplerState objects. 
	class SamplerStateCollection {
	public:
		SamplerStateCollection(){}

		SamplerStateCollection(size_t size) 
			: samplers(size){}

		SamplerStateCollection(std::vector<std::shared_ptr<SamplerState>> const& samplers)
			: samplers(samplers) {}

		std::shared_ptr<SamplerState> operator[](size_t index) {
			if (index >= samplers.size())
				return nullptr;

			return samplers[index];
		}

		constexpr size_t Count() const {
			return samplers.size();
		}

		void Apply(GraphicsDevice const& device);

	public:
		std::vector<std::shared_ptr<SamplerState>> samplers;
	};	
}

#endif