#ifndef XNA_GRAPHICS_SAMPLERSTATE_HPP
#define XNA_GRAPHICS_SAMPLERSTATE_HPP

#include "../default.hpp"

namespace xna {
	class ISamplerState {
	public:
		virtual ~ISamplerState(){}
		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) = 0;
		virtual void Filter(TextureFilter value) = 0;
		virtual void AddressU(TextureAddressMode value) = 0;
		virtual void AddressV(TextureAddressMode value) = 0;
		virtual void AddressW(TextureAddressMode value) = 0;
		virtual void Comparison(ComparisonFunction value) = 0;
		virtual void MipLODBias(float value) = 0;
		virtual void MinLOD(float value) = 0;
		virtual void MaxLOD(float value) = 0;
		virtual void MaxAnisotropy(Uint value) = 0;
		virtual TextureFilter Filter() const = 0;
		virtual TextureAddressMode AddressU() const = 0;
		virtual TextureAddressMode AddressV() const = 0;
		virtual TextureAddressMode AddressW() const = 0;
		virtual ComparisonFunction Comparison() const = 0;
		virtual float MipLODBias() const = 0;
		virtual float MinLOD() const = 0;
		virtual float MaxLOD() const = 0;
		virtual Uint MaxAnisotropy() const = 0;

		static PSamplerState PoinWrap();
		static PSamplerState PointClamp();
		static PSamplerState LinearWrap();
		static PSamplerState LinearClamp();
		static PSamplerState AnisotropicWrap();
		static PSamplerState AnisotropicClamp();
	};
}

#endif