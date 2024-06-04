#ifndef XNA_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_GRAPHICS_SAMPLERSTATE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class SamplerState : GraphicsResource {
	public:
		SamplerState();
		SamplerState(sptr<GraphicsDevice> const& device);
		~SamplerState() override;
		bool Initialize(xna_error_nullarg);
		bool Apply(xna_error_nullarg);
		void Filter(TextureFilter value);
		void AddressU(TextureAddressMode value);
		void AddressV(TextureAddressMode value);
		void AddressW(TextureAddressMode value);
		void Comparison(ComparisonFunction value);
		void MipLODBias(float value);
		void MinLOD(float value);
		void MaxLOD(float value);
		void MaxAnisotropy(Uint value);
		TextureFilter Filter() const;
		TextureAddressMode AddressU() const;
		TextureAddressMode AddressV() const;
		TextureAddressMode AddressW() const;
		ComparisonFunction Comparison() const;
		float MipLODBias() const;
		float MinLOD() const;
		float MaxLOD() const;
		Uint MaxAnisotropy() const;

		static uptr<SamplerState> PoinWrap();
		static uptr<SamplerState> PointClamp();
		static uptr<SamplerState> LinearWrap();
		static uptr<SamplerState> LinearClamp();
		static uptr<SamplerState> AnisotropicWrap();
		static uptr<SamplerState> AnisotropicClamp();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif