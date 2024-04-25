#ifndef XNA_PLATFORM_INDEXBUFFER_DX_HPP
#define XNA_PLATFORM_INDEXBUFFER_DX_HPP

#include "../graphics/gresource.hpp"
#include "../graphics/indexbuffer.hpp"
#include "device-dx.hpp"
#include "dxheaders.hpp"
#include <BufferHelpers.h>
#include <VertexTypes.h>

namespace xna {
	template <typename T>
	class IndexBuffer : public IIndexBuffer, public GraphicsResource {
	public:
		constexpr IndexBuffer(GraphicsDevice* device) : GraphicsResource(device) {}

		constexpr IndexBuffer(GraphicsDevice* device, std::vector<T> const& indices) : data(indices), GraphicsResource(device) {}

		virtual ~IndexBuffer() override {
			if (dxBuffer) {
				dxBuffer->Release();
				dxBuffer = nullptr;
			}
		}

		virtual bool Initialize(xna_error_ptr_arg) override {
			if (!m_device || !m_device->_device || data.empty()) {
				xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
				return false;
			}

			const auto hr = DirectX::CreateStaticBuffer(m_device->_device, data.data(), data.size(), sizeof(T), D3D11_BIND_INDEX_BUFFER, &dxBuffer);

			if (FAILED(hr)) {
				xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
				return false;
			}

			return true;
		}

		virtual bool Apply(xna_error_ptr_arg) override {
			if (!m_device || !m_device->_context || !dxBuffer) {
				xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
				return false;
			}

			m_device->_context->IASetIndexBuffer(dxBuffer, DXGI_FORMAT_R16_UINT, 0);

			return true;
		}

	public:
		ID3D11Buffer* dxBuffer = nullptr;
		std::vector<T> data;
	};
}

#endif