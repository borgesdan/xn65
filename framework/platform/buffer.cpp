#include "graphics/buffer.hpp"
#include "common/numerics.hpp"
#include "platform-dx/dxheaders.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	ConstantBuffer::ConstantBuffer() : GraphicsResource(nullptr){
		impl = uNew<PlatformImplementation>();		
	}

	ConstantBuffer::ConstantBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device){
		impl = uNew<PlatformImplementation>();
	}
	
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

	DataBuffer::DataBuffer() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
	}

	DataBuffer::DataBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
	}

	DataBuffer::~DataBuffer() {
		impl = nullptr;
	}

	bool DataBuffer::Initialize(xna_error_ptr_arg) {
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->_blob) {
			impl->_blob->Release();
			impl->_blob = nullptr;
		}

		return true;
	}

	IndexBuffer::IndexBuffer() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
	}
	
	IndexBuffer::IndexBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
	}

	IndexBuffer::~IndexBuffer() {
		impl = nullptr;
	}

	bool IndexBuffer::Apply(xna_error_ptr_arg) {
		if (!m_device || !m_device->_context || !impl || !impl->dxBuffer) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		m_device->_context->IASetIndexBuffer(impl->dxBuffer, DXGI_FORMAT_R16_UINT, 0);

		return true;
	}
}