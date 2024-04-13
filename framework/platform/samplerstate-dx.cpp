#include "samplerstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	PSamplerState ISamplerState::PoinWrap() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	PSamplerState ISamplerState::PointClamp() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	PSamplerState ISamplerState::LinearWrap() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	PSamplerState ISamplerState::LinearClamp() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	PSamplerState ISamplerState::AnisotropicWrap() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	PSamplerState ISamplerState::AnisotropicClamp() {
		auto state = New<SamplerState>();
		state->_description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->_description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->_description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	bool SamplerState::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._device) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (_samplerState) {
			_samplerState->Release();
			_samplerState = nullptr;
		}

		const auto hr = device._device->CreateSamplerState(&_description, &_samplerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}