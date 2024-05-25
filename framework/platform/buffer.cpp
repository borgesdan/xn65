#include "graphics/buffer.hpp"
#include "common/numerics.hpp"
#include "platform-dx/dxheaders.hpp"
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
		if (!m_device || !m_device->impl->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->_buffer) {
			impl->_buffer->Release();
			impl->_buffer = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateBuffer(
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
		if (!m_device || !m_device->impl->_device) {
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
		if (!m_device || !m_device->impl->_context || !impl || !impl->dxBuffer) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		m_device->impl->_context->IASetIndexBuffer(impl->dxBuffer, DXGI_FORMAT_R16_UINT, 0);

		return true;
	}

	VertexBuffer::VertexBuffer() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
	}

	VertexBuffer::VertexBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
	}

	VertexBuffer::~VertexBuffer() {
		impl = nullptr;
	}

	bool VertexBuffer::Apply(xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->impl->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!impl->dxBuffer) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		UINT stride = impl->size;
		UINT offset = 0;
		m_device->impl->_context->IASetVertexBuffers(0, 1,
			&impl->dxBuffer, &stride, &offset);

		return true;
	}

	VertexInputLayout::VertexInputLayout() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
	}

	VertexInputLayout::VertexInputLayout(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
	}

	VertexInputLayout::~VertexInputLayout() {
		impl = nullptr;
	}

	bool VertexInputLayout::Initialize(DataBuffer& blob, xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->impl->_device || !blob.impl->_blob) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->_inputLayout) {
			impl->_inputLayout->Release();
			impl->_inputLayout = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateInputLayout(
			impl->_description.data(),
			static_cast<UINT>(impl->_description.size()),
			blob.impl->_blob->GetBufferPointer(),
			blob.impl->_blob->GetBufferSize(),
			&impl->_inputLayout);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}