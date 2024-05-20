#include "graphics/buffer.hpp"
#include "common/numerics.hpp"
#include "platform-dx/dxheaders.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	ConstantBuffer::ConstantBuffer() : GraphicsResource(nullptr){}
	ConstantBuffer::ConstantBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device){}
	
	ConstantBuffer::~ConstantBuffer() {
		impl = nullptr;
	}

	bool ConstantBuffer::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->_buffer) {
			impl->_buffer->Release();
			impl->_buffer = nullptr;
		}

		const auto hr = m_device->_device->CreateBuffer(
			&impl->_description,
			&impl->_subResource,
			&impl->_buffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}