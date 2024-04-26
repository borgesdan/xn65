#include "samplerstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool SamplerState::Apply(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!_samplerState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->_context->PSSetSamplers(0, 1, &_samplerState);

		return true;
	}
	uptr<SamplerState> SamplerState::PoinWrap() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::PointClamp() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearWrap() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearClamp() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicWrap() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicClamp() {
		auto state = std::unique_ptr<SamplerState>(new SamplerState());
		state->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	bool SamplerState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (_samplerState) {
			_samplerState->Release();
			_samplerState = nullptr;
		}

		const auto hr = m_device->_device->CreateSamplerState(&_description, &_samplerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}