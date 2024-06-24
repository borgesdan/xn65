#include "xna/graphics/buffer.hpp"
#include "xna/common/numerics.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	ConstantBuffer::ConstantBuffer() : GraphicsResource(nullptr){
		impl = unew<PlatformImplementation>();		
	}

	ConstantBuffer::ConstantBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device){
		impl = unew<PlatformImplementation>();
	}
	
	ConstantBuffer::~ConstantBuffer() {
		impl = nullptr;
	}

	bool ConstantBuffer::Initialize()
	{
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
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
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	DataBuffer::DataBuffer() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
	}

	DataBuffer::DataBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	DataBuffer::~DataBuffer() {
		impl = nullptr;
	}

	bool DataBuffer::Initialize() {
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
		}

		if (impl->_blob) {
			impl->_blob->Release();
			impl->_blob = nullptr;
		}

		return true;
	}

	IndexBuffer::IndexBuffer() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
	}
	
	IndexBuffer::IndexBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	IndexBuffer::~IndexBuffer() {
		impl = nullptr;
	}

	bool IndexBuffer::Apply() {
		if (!m_device || !m_device->impl->_context || !impl || !impl->dxBuffer) {
			Exception::Throw(ExMessage::ApplyComponent);
		}

		m_device->impl->_context->IASetIndexBuffer(impl->dxBuffer, DXGI_FORMAT_R16_UINT, 0);

		return true;
	}

	VertexBuffer::VertexBuffer() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
	}

	VertexBuffer::VertexBuffer(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	VertexBuffer::~VertexBuffer() {
		impl = nullptr;
	}

	bool VertexBuffer::Apply() {
		if (!impl || !m_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::ApplyComponent);
		}

		if (!impl->dxBuffer) {
			Exception::Throw(ExMessage::UnintializedComponent);
		}

		UINT stride = impl->size;
		UINT offset = 0;
		m_device->impl->_context->IASetVertexBuffers(0, 1,
			&impl->dxBuffer, &stride, &offset);

		return true;
	}

	VertexInputLayout::VertexInputLayout() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
	}

	VertexInputLayout::VertexInputLayout(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	VertexInputLayout::~VertexInputLayout() {
		impl = nullptr;
	}

	bool VertexInputLayout::Initialize(DataBuffer& blob) {
		if (!impl || !m_device || !m_device->impl->_device || !blob.impl->_blob) {
			Exception::Throw(ExMessage::InitializeComponent);
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
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}
}