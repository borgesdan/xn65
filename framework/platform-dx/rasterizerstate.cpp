#include "xna/graphics/rasterizerstate.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {

	RasterizerState::RasterizerState() : RasterizerState(nullptr){}

	RasterizerState::RasterizerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
		impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		impl->dxDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		impl->dxDescription.MultisampleEnable = true;
		impl->dxDescription.DepthBias = 0;
		impl->dxDescription.SlopeScaledDepthBias = 0;
		impl->dxDescription.ScissorEnable = false;
	}
	
	bool RasterizerState::Initialize()
	{
		if (!impl || !m_device || !m_device->impl->_device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (impl->dxRasterizerState) {
			impl->dxRasterizerState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateRasterizerState(
			&impl->dxDescription, 
			impl->dxRasterizerState.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		return true;
	}	

	bool RasterizerState::Apply()
	{
		if (!impl || !m_device || !m_device->impl->_context) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (!impl->dxRasterizerState) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		m_device->impl->_context->RSSetState(impl->dxRasterizerState.Get());

		return true;
	}

	bool RasterizerState::ScissorTestEnable() const {
		return impl->dxDescription.ScissorEnable;
	}

	void  RasterizerState::ScissorTestEnable(bool value) {
		impl->dxDescription.ScissorEnable = value;
	}

	bool RasterizerState::MultiSampleAntiAlias() const {
		return impl->dxDescription.MultisampleEnable;
	}

	void  RasterizerState::MultiSampleAntiAlias(bool value) {
		impl->dxDescription.MultisampleEnable = value;
	}

	float RasterizerState::DepthBias() const {
		return impl->dxDescription.DepthBias;
	}

	void RasterizerState::DepthBias(float value) {
		impl->dxDescription.DepthBias = value;
	}

	float RasterizerState::SlopeScaleDepthBias() const {
		return impl->dxDescription.SlopeScaledDepthBias;
	}

	void RasterizerState::SlopeScaleDepthBias(float value) {
		impl->dxDescription.SlopeScaledDepthBias = value;
	}

	uptr<RasterizerState> RasterizerState::CullNone()
	{
		auto raster = unew<RasterizerState>();
		raster->impl->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		raster->impl->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullClockwise()
	{
		auto raster = unew<RasterizerState>();
		raster->impl->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		raster->impl->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullCounterClockwise()
	{
		auto raster = unew<RasterizerState>();
		raster->impl->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		raster->impl->dxDescription.DepthClipEnable = true;
		return raster;
	}

	xna::CullMode RasterizerState::CullMode() const {
		if (!impl)
			return xna::CullMode::None;

		return static_cast<xna::CullMode>(static_cast<int>(impl->dxDescription.CullMode) - 1);
	}

	void RasterizerState::CullMode(xna::CullMode value) {
		if (!impl)
			return;

		impl->dxDescription.CullMode = static_cast<D3D11_CULL_MODE>(static_cast<int>(value) + 1);
	}

	xna::FillMode RasterizerState::FillMode() const {
		if (!impl)
			return xna::FillMode::WireFrame;

		return static_cast<xna::FillMode>(static_cast<int>(impl->dxDescription.FillMode) - 2);
	}

	void RasterizerState::FillMode(xna::FillMode value) {
		if (!impl)
			return;

		impl->dxDescription.FillMode = static_cast<D3D11_FILL_MODE>(static_cast<int>(value) + 2);
	}
}
