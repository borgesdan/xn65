#ifndef XNA_PLATFORM_SAMPLERSTATE_DX_HPP
#define XNA_PLATFORM_SAMPLERSTATE_DX_HPP

#include "../graphics/samplerstate.hpp"
#include "dxheaders.hpp"

namespace xna {
	class SamplerState : public ISamplerState {
	public:
		SamplerState() {
			_description.MaxAnisotropy = 4;			
		}

		virtual ~SamplerState() override {
			if (_samplerState) {
				_samplerState->Release();
				_samplerState = nullptr;
			}
		}

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
			switch (value)
			{
			case xna::ComparisonFunction::Never:
				_description.ComparisonFunc = D3D11_COMPARISON_NEVER;
				break;
			case xna::ComparisonFunction::Less:
				_description.ComparisonFunc = D3D11_COMPARISON_LESS;
				break;
			case xna::ComparisonFunction::Equal:
				_description.ComparisonFunc = D3D11_COMPARISON_EQUAL;
				break;
			case xna::ComparisonFunction::LessEquals:
				_description.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
				break;
			case xna::ComparisonFunction::Greater:
				_description.ComparisonFunc = D3D11_COMPARISON_GREATER;
				break;
			case xna::ComparisonFunction::NotEqual:
				_description.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
				break;
			case xna::ComparisonFunction::GreaterEqual:
				_description.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
				break;
			case xna::ComparisonFunction::Always:
				_description.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				break;
			default:
				_description.ComparisonFunc = D3D11_COMPARISON_NEVER;
				break;
			}
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
			switch (_description.ComparisonFunc)
			{
			case D3D11_COMPARISON_NEVER :
				return xna::ComparisonFunction::Never;
			case D3D11_COMPARISON_LESS:
				return xna::ComparisonFunction::Less;				
			case D3D11_COMPARISON_EQUAL:
				return xna::ComparisonFunction::Equal;
			case D3D11_COMPARISON_LESS_EQUAL:
				return xna::ComparisonFunction::LessEquals;				
			case D3D11_COMPARISON_GREATER:
				return xna::ComparisonFunction::Greater;				
			case D3D11_COMPARISON_NOT_EQUAL:
				return xna::ComparisonFunction::NotEqual;				
			case D3D11_COMPARISON_GREATER_EQUAL:
				return xna::ComparisonFunction::GreaterEqual;
			case D3D11_COMPARISON_ALWAYS:
				return xna::ComparisonFunction::Always;
			default:
				return xna::ComparisonFunction::Never;				
			}
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

	public:
		ID3D11SamplerState* _samplerState = nullptr;
		D3D11_SAMPLER_DESC _description{};

	public:
		static constexpr void ConvertAddressMode(TextureAddressMode value, D3D11_TEXTURE_ADDRESS_MODE& target) {
			switch (value)
			{
			case xna::TextureAddressMode::Wrap:
				target = D3D11_TEXTURE_ADDRESS_WRAP;
				break;
			case xna::TextureAddressMode::Clamp:
				target = D3D11_TEXTURE_ADDRESS_CLAMP;
				break;
			case xna::TextureAddressMode::Mirror:
				target = D3D11_TEXTURE_ADDRESS_MIRROR;
				break;
			case xna::TextureAddressMode::Border:
				target = D3D11_TEXTURE_ADDRESS_BORDER;
				break;
			case xna::TextureAddressMode::MirrorOnce:
				target = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
				break;
			default:
				target = D3D11_TEXTURE_ADDRESS_WRAP;				
				break;
			}			
		}

		static constexpr void ConvertAddressMode(D3D11_TEXTURE_ADDRESS_MODE value, TextureAddressMode& target) {
			switch (value)
			{
			case D3D11_TEXTURE_ADDRESS_WRAP:
				target = TextureAddressMode::Wrap;
			case D3D11_TEXTURE_ADDRESS_CLAMP:
				target = TextureAddressMode::Clamp;
			case D3D11_TEXTURE_ADDRESS_MIRROR:
				target = TextureAddressMode::Mirror;
			case D3D11_TEXTURE_ADDRESS_BORDER:
				target = TextureAddressMode::Border;
			case D3D11_TEXTURE_ADDRESS_MIRROR_ONCE:
				target = TextureAddressMode::MirrorOnce;
			default:
				target = TextureAddressMode::Wrap;
				break;
			}
		}
	};
}

#endif