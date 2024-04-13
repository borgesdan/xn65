#include "rasterizerstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool RasterizerState::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		const auto hr = device._device->CreateRasterizerState(&_description, &_rasterizerState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		return true;
	}
}
