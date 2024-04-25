#include "rasterizerstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool RasterizerState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		const auto hr = m_device->_device->CreateRasterizerState(&dxDescription, &dxRasterizerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}	

	bool RasterizerState::Apply(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!dxRasterizerState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->_context->RSSetState(dxRasterizerState);

		return true;
	}

	uptr<RasterizerState> RasterizerState::CullNone()
	{
		auto raster = std::unique_ptr<RasterizerState>(new RasterizerState());
		raster->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		raster->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullClockwise()
	{
		auto raster = std::unique_ptr<RasterizerState>(new RasterizerState());
		raster->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		raster->dxDescription.DepthClipEnable = true;
		return raster;
	}

	uptr<RasterizerState> RasterizerState::CullCounterClockwise()
	{
		auto raster = std::unique_ptr<RasterizerState>(new RasterizerState());
		raster->dxDescription.FillMode = D3D11_FILL_SOLID;
		raster->dxDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		raster->dxDescription.DepthClipEnable = true;
		return raster;
	}
}
