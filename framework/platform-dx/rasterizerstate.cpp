#include "graphics/rasterizerstate.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {

	RasterizerState::RasterizerState() : GraphicsResource(nullptr){
		impl = unew<PlatformImplementation>();
	}

	RasterizerState::RasterizerState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	RasterizerState::~RasterizerState() {
		impl = nullptr;
	}

	bool RasterizerState::Initialize(xna_error_ptr_arg)
	{
		if (!impl || !m_device || !m_device->impl->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->dxRasterizerState) {
			impl->dxRasterizerState->Release();
			impl->dxRasterizerState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateRasterizerState(
			&impl->dxDescription, 
			&impl->dxRasterizerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}	

	bool RasterizerState::Apply(xna_error_ptr_arg)
	{
		if (!impl || !m_device || !m_device->impl->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!impl->dxRasterizerState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->impl->_context->RSSetState(impl->dxRasterizerState);

		return true;
	}

	uptr<RasterizerState> RasterizerState::CullNone()
	{
		auto raster = uNew<RasterizerState>();
		raster->impl->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		raster->impl->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullClockwise()
	{
		auto raster = uNew<RasterizerState>();
		raster->impl->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->impl->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		raster->impl->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullCounterClockwise()
	{
		auto raster = uNew<RasterizerState>();
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
