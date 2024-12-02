#include "xna/graphics/samplerstate.hpp"
#include "xna/graphics/samplerstate.hpp"
#include "xna-dx/framework.hpp"

namespace xna {
	SamplerState::SamplerState() : SamplerState(nullptr){}

	SamplerState::SamplerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		Implementation = unew<SamplerStateImplementation>();
	}

	bool SamplerState::Initialize()
	{
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device) {
			throw csharp::InvalidOperationException();
		}

		if (Implementation->SamplerState) {
			Implementation->SamplerState = nullptr;
		}

		const auto hr = BaseGraphicsDevice->Implementation->Device->CreateSamplerState(
			&Implementation->Description,
			Implementation->SamplerState.GetAddressOf());

		if (FAILED(hr)) {
			throw csharp::InvalidOperationException();
		}

		return true;
	}

	bool SamplerState::Apply()
	{
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Context) {
			throw csharp::InvalidOperationException();
		}

		if (!Implementation->SamplerState) {
			throw csharp::InvalidOperationException();
		}

		BaseGraphicsDevice->Implementation->Context->PSSetSamplers(0, 1, Implementation->SamplerState.GetAddressOf());

		return true;
	}

	void SamplerStateCollection::Apply(GraphicsDevice const& device) {
		if (samplers.empty())
			return;

		if (!device.Implementation || !device.Implementation->Device || !device.Implementation->Context) {
			throw csharp::InvalidOperationException();
		}

		std::vector<ID3D11SamplerState*> states(samplers.size());

		for (size_t i = 0; i < samplers.size(); ++i) {
			const auto& current = samplers[0];

			if (!current || !current->Implementation || !current->Implementation->SamplerState)
				throw csharp::InvalidOperationException();

			states[i] = current->Implementation->SamplerState.Get();
			states[i]->AddRef();
		}

		device.Implementation->Context->PSSetSamplers(
			0, 
			static_cast<UINT>(states.size()),
			states.data());

		for (size_t i = 0; i < samplers.size(); ++i) {			
			states[i]->Release();
			states[i] = nullptr;
		}
	}

	uptr<SamplerState> SamplerState::PoinWrap() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::PointClamp() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearWrap() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::LinearClamp() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicWrap() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return state;
	}

	uptr<SamplerState> SamplerState::AnisotropicClamp() {
		auto state = unew<SamplerState>();
		state->Implementation->Description.Filter = D3D11_FILTER_ANISOTROPIC;
		state->Implementation->Description.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		state->Implementation->Description.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		return state;
	}	

	void SamplerState::Filter(TextureFilter value) {
		switch (value)
		{
		case xna::TextureFilter::Linear:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case xna::TextureFilter::Point:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case xna::TextureFilter::Anisotropic:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
			break;
		case xna::TextureFilter::LinearMipPoint:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case xna::TextureFilter::PointMipLinear:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinLinearMagPointMipLinear:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinLinearMagPointMipPoint:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case xna::TextureFilter::MinPointMagLinearMipLinear:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case xna::TextureFilter::MinPointMagLinearMipPoint:
			Implementation->Description.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		default:
			break;
		}
	}

	void SamplerState::AddressU(TextureAddressMode value) {
		Implementation->Description.AddressU = DxHelpers::TextureAddresModeToDx(value);
	}

	void SamplerState::AddressV(TextureAddressMode value) {
		Implementation->Description.AddressV = DxHelpers::TextureAddresModeToDx(value);
	}

	void SamplerState::AddressW(TextureAddressMode value) {
		Implementation->Description.AddressW = DxHelpers::TextureAddresModeToDx(value);
	}

	void SamplerState::Comparison(ComparisonFunction value) {
		Implementation->Description.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int>(value) + 1);
	}

	void SamplerState::MipMapLevelOfDetailBias(float value) {
		Implementation->Description.MipLODBias = value;
	}

	void SamplerState::MinMipLevel(float value) {
		Implementation->Description.MinLOD = value;
	}

	void SamplerState::MaxMipLevel (float value) {
		Implementation->Description.MaxLOD = value;
	}

	void SamplerState::MaxAnisotropy(Uint value) {
		Implementation->Description.MaxAnisotropy = static_cast<UINT>(value);
	}

	TextureFilter SamplerState::Filter() const {
		switch (Implementation->Description.Filter)
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
		return DxHelpers::TextureAddresModeToXna(Implementation->Description.AddressU);
	}

	TextureAddressMode SamplerState::AddressV() const {		
		return DxHelpers::TextureAddresModeToXna(Implementation->Description.AddressV);
	}

	TextureAddressMode SamplerState::AddressW() const {		
		return DxHelpers::TextureAddresModeToXna(Implementation->Description.AddressW);
	}

	ComparisonFunction SamplerState::Comparison() const {
		return static_cast<ComparisonFunction>(Implementation->Description.ComparisonFunc - 1);
	}

	float SamplerState::MipMapLevelOfDetailBias() const {
		return Implementation->Description.MipLODBias;
	}

	float SamplerState::MinMipLevel() const {
		return Implementation->Description.MinLOD;
	}

	float SamplerState::MaxMipLevel() const {
		return Implementation->Description.MaxLOD;
	}

	Uint SamplerState::MaxAnisotropy() const {
		return Implementation->Description.MaxAnisotropy;
	}
}