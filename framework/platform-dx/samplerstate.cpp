#include "xna/graphics/samplerstate.hpp"
#include "xna/graphics/samplerstate.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	SamplerState::SamplerState() : SamplerState(nullptr){}

	SamplerState::SamplerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	bool SamplerState::Initialize()
	{
		if (!impl || !m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
		}

		if (impl->_samplerState) {
			impl->_samplerState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateSamplerState(
			&impl->_description,
			impl->_samplerState.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	bool SamplerState::Apply()
	{
		if (!impl || !m_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		if (!impl->_samplerState) {
			Exception::Throw(ExMessage::UnintializedComponent);
		}

		m_device->impl->_context->PSSetSamplers(0, 1, impl->_samplerState.GetAddressOf());

		return true;
	}

	void SamplerStateCollection::Apply(GraphicsDevice const& device) {
		if (samplers.empty())
			return;

		if (!device.impl || !device.impl->_device || !device.impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		std::vector<ID3D11SamplerState*> states(samplers.size());

		for (size_t i = 0; i < samplers.size(); ++i) {
			const auto& current = samplers[0];

			if (!current || !current->impl || !current->impl->_samplerState)
				Exception::Throw(ExMessage::InvalidOperation);

			states[i] = current->impl->_samplerState.Get();
			states[i]->AddRef();
		}

		device.impl->_context->PSSetSamplers(0, states.size(), states.data());

		for (size_t i = 0; i < samplers.size(); ++i) {			
			states[i]->Release();
			states[i] = nullptr;
		}
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

	void SamplerState::MipMapLevelOfDetailBias(float value) {
		impl->_description.MipLODBias = value;
	}

	void SamplerState::MinMipLevel(float value) {
		impl->_description.MinLOD = value;
	}

	void SamplerState::MaxMipLevel (float value) {
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

	float SamplerState::MipMapLevelOfDetailBias() const {
		return impl->_description.MipLODBias;
	}

	float SamplerState::MinMipLevel() const {
		return impl->_description.MinLOD;
	}

	float SamplerState::MaxMipLevel() const {
		return impl->_description.MaxLOD;
	}

	Uint SamplerState::MaxAnisotropy() const {
		return impl->_description.MaxAnisotropy;
	}
}