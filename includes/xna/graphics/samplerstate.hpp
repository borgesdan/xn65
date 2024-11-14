#ifndef XNA_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_GRAPHICS_SAMPLERSTATE_HPP

#include "../default.hpp"
#include "shared.hpp"
#include "gresource.hpp"

namespace xna {
	//Contains sampler state, which determines how to sample texture data. 
	class SamplerState : public GraphicsResource {
	public:
		SamplerState();
		SamplerState(sptr<GraphicsDevice> const& device);
		
		//Gets or sets the maximum anisotropy. The default value is 0.
		void MaxAnisotropy(Uint value);
		//Gets or sets the maximum anisotropy. The default value is 0.
		Uint MaxAnisotropy() const;
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
		static uptr<SamplerState> PoinWrap();
		//Contains default state for point filtering and texture coordinate clamping.
		static uptr<SamplerState> PointClamp();
		//Contains default state for linear filtering and texture coordinate wrapping.
		static uptr<SamplerState> LinearWrap();
		//Contains default state for linear filtering and texture coordinate clamping.
		static uptr<SamplerState> LinearClamp();
		//Contains default state for anisotropic filtering and texture coordinate wrapping.
		static uptr<SamplerState> AnisotropicWrap();
		//Contains default state for anisotropic filtering and texture coordinate clamping.
		static uptr<SamplerState> AnisotropicClamp();

		ComparisonFunction Comparison() const;
		void Comparison(ComparisonFunction value);

		bool Initialize();
		bool Apply();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	using PSamplerState = sptr<SamplerState>;

	//Collection of SamplerState objects. 
	class SamplerStateCollection {
	public:
		SamplerStateCollection(){}

		SamplerStateCollection(size_t size) 
			: samplers(size){}

		SamplerStateCollection(std::vector<PSamplerState> const& samplers) 
			: samplers(samplers) {}

		PSamplerState operator[](size_t index) {
			if (index >= samplers.size())
				return nullptr;

			return samplers[index];
		}

		constexpr size_t Count() const {
			return samplers.size();
		}

		void Apply(GraphicsDevice const& device);

	public:
		std::vector<PSamplerState> samplers;
	};

	using PSamplerStateCollection = sptr<SamplerStateCollection>;
}

#endif