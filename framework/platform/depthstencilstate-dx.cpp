#include "depthstencilstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool DepthStencilState::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._device) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (_depthStencil) {
			_depthStencil->Release();
			_depthStencil = nullptr;
		}

		const auto hr = device._device->CreateDepthStencilState(&_description, &_depthStencil);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool DepthStencilState::Apply(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._context) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (!_depthStencil) {
			const auto init = Initialize(device, err);
			if (!init) return false;
		}

		device._context->OMSetDepthStencilState(_depthStencil, 0);

		return true;
	}

	PDepthStencilState IDepthStencilState::None() {
		auto stencil = New<DepthStencilState>();
		stencil->_description.DepthEnable = false;
		stencil->_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	PDepthStencilState IDepthStencilState::Default() {
		auto stencil = New<DepthStencilState>();
		stencil->_description.DepthEnable = true;
		stencil->_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return stencil;
	}

	PDepthStencilState IDepthStencilState::DepthRead() {
		auto stencil = New<DepthStencilState>();
		stencil->_description.DepthEnable = true;
		stencil->_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}
}