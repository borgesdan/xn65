#include "constbuffer-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool ConstantBuffer::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._device) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (_buffer) {
			_buffer->Release();
			_buffer = nullptr;
		}

		const auto hr = device._device->CreateBuffer(
			&_description,
			&_subResource,
			&_buffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}