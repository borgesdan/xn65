#include "xna/graphics/samplerstate.hpp"
#include "xna/graphics/samplerstate.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	SamplerState::SamplerState() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();		
	}

	SamplerState::SamplerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	SamplerState::~SamplerState() {
		impl = nullptr;
	}

	bool SamplerState::Initialize(xna_error_ptr_arg)
	{
		if (!impl || !m_device || !m_device->impl->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->_samplerState) {
			impl->_samplerState->Release();
			impl->_samplerState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateSamplerState(
			&impl->_description,
			&impl->_samplerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool SamplerState::Apply(xna_error_ptr_arg)
	{
		if (!impl || !m_device || !m_device->impl->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!impl->_samplerState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->impl->_context->PSSetSamplers(0, 1, &impl->_samplerState);

		return true;
	}
	uptr<SamplerState> SamplerState::PoinWrap() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::PointClamp() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearWrap() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearClamp() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicWrap() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicClamp() {
		auto state = unew<SamplerState>();
		state->impl->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->impl->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->impl->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}	

	void SamplerState::Filter(TextureFilter value) {
		switch (value)
		{
		case xna::TextureFilter::Linear:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case xna::TextureFilter::Point:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case xna::TextureFilter::Anisotropic:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
			break;
		case xna::TextureFilter::LinearMipPoint:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case xna::TextureFilter::PointMipLinear:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinLinearMagPointMipLinear:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinLinearMagPointMipPoint:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case xna::TextureFilter::MinPointMagLinearMipLinear:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinPointMagLinearMipPoint:
			impl->_description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		default:
			break;
		}
	}

	void SamplerState::AddressU(TextureAddressMode value) {
		DxHelpers::ConvertAddressMode(value, impl->_description.AddressU);
	}

	void SamplerState::AddressV(TextureAddressMode value) {
		DxHelpers::ConvertAddressMode(value, impl->_description.AddressV);
	}

	void SamplerState::AddressW(TextureAddressMode value) {
		DxHelpers::ConvertAddressMode(value, impl->_description.AddressW);
	}

	void SamplerState::Comparison(ComparisonFunction value) {
		impl->_description.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int>(value) + 1);
	}

	void SamplerState::MipLODBias(float value) {
		impl->_description.MipLODBias = value;
	}

	void SamplerState::MinLOD(float value) {
		impl->_description.MinLOD = value;
	}

	void SamplerState::MaxLOD(float value) {
		impl->_description.MaxLOD = value;
	}

	void SamplerState::MaxAnisotropy(Uint value) {
		impl->_description.MaxAnisotropy = static_cast<UINT>(value);
	}

	TextureFilter SamplerState::Filter() const {
		switch (impl->_description.Filter)
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

	TextureAddressMode SamplerState::AddressU() const {
		TextureAddressMode mode;
		DxHelpers::ConvertAddressMode(impl->_description.AddressU, mode);
		return mode;
	}

	TextureAddressMode SamplerState::AddressV() const {
		TextureAddressMode mode;
		DxHelpers::ConvertAddressMode(impl->_description.AddressV, mode);
		return mode;
	}

	TextureAddressMode SamplerState::AddressW() const {
		TextureAddressMode mode;
		DxHelpers::ConvertAddressMode(impl->_description.AddressW, mode);
		return mode;
	}

	ComparisonFunction SamplerState::Comparison() const {
		return static_cast<ComparisonFunction>(impl->_description.ComparisonFunc - 1);
	}

	float SamplerState::MipLODBias() const {
		return impl->_description.MipLODBias;
	}

	float SamplerState::MinLOD() const {
		return impl->_description.MinLOD;
	}

	float SamplerState::MaxLOD() const {
		return impl->_description.MaxLOD;
	}

	Uint SamplerState::MaxAnisotropy() const {
		return impl->_description.MaxAnisotropy;
	}
}