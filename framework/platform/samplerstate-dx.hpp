#ifndef XNA_PLATFORM_SAMPLERSTATE_DX_HPP
#define XNA_PLATFORM_SAMPLERSTATE_DX_HPP

#include "../graphics/samplerstate.hpp"
#include "../graphics/gresource.hpp"
#include "dxheaders.hpp"

namespace xna {
	class SamplerState : public ISamplerState, public GraphicsResource {
	public:
		SamplerState(GraphicsDevice* device) : GraphicsResource(device) {
			_description.MaxAnisotropy = 4;
		}

		virtual ~SamplerState() override {
			if (_samplerState) {
				_samplerState->Release();
				_samplerState = nullptr;
			}
		}

		virtual bool Initialize(xna_error_nullarg) override;
		virtual bool Apply(xna_error_nullarg) override;

		virtual constexpr void Filter(TextureFilter value) override {
			switch (value)
			{
			case xna::TextureFilter::Linear:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			case xna::TextureFilter::Point:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
			case xna::TextureFilter::Anisotropic:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
				break;
			case xna::TextureFilter::LinearMipPoint:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				break;
			case xna::TextureFilter::PointMipLinear:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				break;
			case xna::TextureFilter::MinLinearMagPointMipLinear:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			case xna::TextureFilter::MinLinearMagPointMipPoint:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				break;
			case xna::TextureFilter::MinPointMagLinearMipLinear:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				break;
			case xna::TextureFilter::MinPointMagLinearMipPoint:
				_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			default:
				break;
			}
		}

		virtual constexpr void AddressU(TextureAddressMode value) override {
			ConvertAddressMode(value, _description.AddressU);
		}

		virtual constexpr void AddressV(TextureAddressMode value) override {
			ConvertAddressMode(value, _description.AddressV);
		}

		virtual constexpr void AddressW(TextureAddressMode value) override {
			ConvertAddressMode(value, _description.AddressW);
		}

		virtual constexpr void Comparison(ComparisonFunction value) override {
			_description.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int>(value) + 1);
		}

		virtual constexpr void MipLODBias(float value) override {
			_description.MipLODBias = value;
		}

		virtual constexpr void MinLOD(float value) override {
			_description.MinLOD = value;
		}

		virtual constexpr void MaxLOD(float value) override {
			_description.MaxLOD = value;
		}

		virtual void MaxAnisotropy(Uint value) override {
			_description.MaxAnisotropy = static_cast<UINT>(value);
		}

		virtual constexpr TextureFilter Filter() const override {
			switch (_description.Filter)
			{
			case D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR:
				return xna::TextureFilter::Linear;
			case D3D11_FILTER_MIN_MAG_MIP_POINT:
				return xna::TextureFilter::Point;
			case D3D11_FILTER_ANISOTROPIC:
				return xna::TextureFilter::Anisotropic;
			case D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT:
				return xna::TextureFilter::LinearMipPoint;
			case D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR:
				return xna::TextureFilter::PointMipLinear;
			case D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return xna::TextureFilter::MinLinearMagPointMipLinear;
			case D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT:
				return xna::TextureFilter::MinLinearMagPointMipPoint;
			case D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR:
				return xna::TextureFilter::MinPointMagLinearMipLinear;
			case D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return xna::TextureFilter::MinPointMagLinearMipPoint;
			default:
				return xna::TextureFilter::Linear;
			}
		}

		virtual constexpr TextureAddressMode AddressU() const override {
			TextureAddressMode mode;
			ConvertAddressMode(_description.AddressU, mode);
			return mode;
		}

		virtual constexpr TextureAddressMode AddressV() const override {
			TextureAddressMode mode;
			ConvertAddressMode(_description.AddressV, mode);
			return mode;
		}

		virtual constexpr TextureAddressMode AddressW() const override {
			TextureAddressMode mode;
			ConvertAddressMode(_description.AddressW, mode);
			return mode;
		}

		virtual ComparisonFunction Comparison() const override {
			return static_cast<ComparisonFunction>(_description.ComparisonFunc - 1);
		}

		virtual constexpr float MipLODBias() const override {
			return _description.MipLODBias;
		}

		virtual constexpr float MinLOD() const override {
			return _description.MinLOD;
		}

		virtual constexpr float MaxLOD() const override {
			return _description.MaxLOD;
		}

		virtual constexpr Uint MaxAnisotropy() const override {
			return _description.MaxAnisotropy;
		}

		static uptr<SamplerState> PoinWrap();
		static uptr<SamplerState> PointClamp();
		static uptr<SamplerState> LinearWrap();
		static uptr<SamplerState> LinearClamp();
		static uptr<SamplerState> AnisotropicWrap();
		static uptr<SamplerState> AnisotropicClamp();

	public:
		ID3D11SamplerState* _samplerState = nullptr;
		D3D11_SAMPLER_DESC _description{};

	public:
		static constexpr void ConvertAddressMode(TextureAddressMode value, D3D11_TEXTURE_ADDRESS_MODE& target) {
			target = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(static_cast<int>(value) + 1);
		}

		static constexpr void ConvertAddressMode(D3D11_TEXTURE_ADDRESS_MODE value, TextureAddressMode& target) {
			target = static_cast<TextureAddressMode>(value - 1);
		}

	private:
		SamplerState() : GraphicsResource(nullptr) {}
	};
}

#endif