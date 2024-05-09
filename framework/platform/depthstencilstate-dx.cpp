#include "platform-dx/depthstencilstate-dx.hpp"
#include "platform-dx/device-dx.hpp"

namespace xna {
	bool DepthStencilState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (dxDepthStencil) {
			dxDepthStencil->Release();
			dxDepthStencil = nullptr;
		}

		const auto hr = m_device->_device->CreateDepthStencilState(&dxDescription, &dxDepthStencil);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool DepthStencilState::Apply(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!dxDepthStencil) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->_context->OMSetDepthStencilState(dxDepthStencil, 0);

		return true;
	}

	uptr<DepthStencilState> DepthStencilState::None() {
		auto stencil = std::unique_ptr<DepthStencilState>(new DepthStencilState());
		stencil->dxDescription.DepthEnable = false;
		stencil->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::Default() {
		auto stencil = std::unique_ptr<DepthStencilState>(new DepthStencilState());
		stencil->dxDescription.DepthEnable = true;
		stencil->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::DepthRead() {
		auto stencil = std::unique_ptr<DepthStencilState>(new DepthStencilState());
		stencil->dxDescription.DepthEnable = true;
		stencil->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}
}