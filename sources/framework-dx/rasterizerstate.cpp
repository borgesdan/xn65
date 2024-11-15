#include "xna/graphics/rasterizerstate.hpp"
#include "xna-dx/framework.hpp"

namespace xna {

	RasterizerState::RasterizerState() : RasterizerState(nullptr){}

	RasterizerState::RasterizerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		Implementation = unew<RasterizerStateImplementation>();
		Implementation->Description.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Implementation->Description.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		Implementation->Description.MultisampleEnable = true;
		Implementation->Description.DepthBias = 0;
		Implementation->Description.SlopeScaledDepthBias = 0;
		Implementation->Description.ScissorEnable = false;
	}
	
	bool RasterizerState::Initialize()
	{
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (Implementation->RasterizerState) {
			Implementation->RasterizerState = nullptr;
		}

		const auto hr = BaseGraphicsDevice->Implementation->Device->CreateRasterizerState(
			&Implementation->Description,
			Implementation->RasterizerState.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		return true;
	}	

	bool RasterizerState::Apply()
	{
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Context) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (!Implementation->RasterizerState) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		BaseGraphicsDevice->Implementation->Context->RSSetState(Implementation->RasterizerState.Get());

		return true;
	}

	bool RasterizerState::ScissorTestEnable() const {
		return Implementation->Description.ScissorEnable;
	}

	void  RasterizerState::ScissorTestEnable(bool value) {
		Implementation->Description.ScissorEnable = value;
	}

	bool RasterizerState::MultiSampleAntiAlias() const {
		return Implementation->Description.MultisampleEnable;
	}

	void  RasterizerState::MultiSampleAntiAlias(bool value) {
		Implementation->Description.MultisampleEnable = value;
	}

	float RasterizerState::DepthBias() const {
		return static_cast<float>(Implementation->Description.DepthBias);
	}

	void RasterizerState::DepthBias(float value) {
		Implementation->Description.DepthBias = static_cast<INT>(value);
	}

	float RasterizerState::SlopeScaleDepthBias() const {
		return Implementation->Description.SlopeScaledDepthBias;
	}

	void RasterizerState::SlopeScaleDepthBias(float value) {
		Implementation->Description.SlopeScaledDepthBias = value;
	}

	uptr<RasterizerState> RasterizerState::CullNone()
	{
		auto raster = unew<RasterizerState>();
		raster->Implementation->Description.FillMode = D3D11_FILL_SOLID;
		raster->Implementation->Description.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		raster->Implementation->Description.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullClockwise()
	{
		auto raster = unew<RasterizerState>();
		raster->Implementation->Description.FillMode = D3D11_FILL_SOLID;
		raster->Implementation->Description.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		raster->Implementation->Description.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullCounterClockwise()
	{
		auto raster = unew<RasterizerState>();
		raster->Implementation->Description.FillMode = D3D11_FILL_SOLID;
		raster->Implementation->Description.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		raster->Implementation->Description.DepthClipEnable = true;
		return raster;
	}

	xna::CullMode RasterizerState::CullMode() const {
		if (!Implementation)
			return xna::CullMode::None;

		return static_cast<xna::CullMode>(static_cast<int>(Implementation->Description.CullMode) - 1);
	}

	void RasterizerState::CullMode(xna::CullMode value) {
		if (!Implementation)
			return;

		Implementation->Description.CullMode = static_cast<D3D11_CULL_MODE>(static_cast<int>(value) + 1);
	}

	xna::FillMode RasterizerState::FillMode() const {
		if (!Implementation)
			return xna::FillMode::WireFrame;

		return static_cast<xna::FillMode>(static_cast<int>(Implementation->Description.FillMode) - 2);
	}

	void RasterizerState::FillMode(xna::FillMode value) {
		if (!Implementation)
			return;

		Implementation->Description.FillMode = static_cast<D3D11_FILL_MODE>(static_cast<int>(value) + 2);
	}
}
