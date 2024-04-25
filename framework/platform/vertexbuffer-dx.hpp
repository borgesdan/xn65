#ifndef XNA_PLATFORM_VERTEXBUFFER_DX_HPP
#define XNA_PLATFORM_VERTEXBUFFER_DX_HPP

#include "../graphics/vertexbuffer.hpp"
#include "../graphics/vertexposition.hpp"
#include "dxheaders.hpp"
#include <BufferHelpers.h>
#include <VertexTypes.h>
#include "device-dx.hpp"
#include "../graphics/gresource.hpp"

namespace xna {
	template <typename T>
	class VertexBuffer : public IVertexBuffer, public GraphicsResource {
	public:
		constexpr VertexBuffer(GraphicsDevice* device) : GraphicsResource(device){}

		constexpr VertexBuffer(GraphicsDevice* device, std::vector<T> const& vertices) : data(vertices), GraphicsResource(device) {}

		virtual ~VertexBuffer() override {
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

			const auto hr = DirectX::CreateStaticBuffer(m_device->_device, data.data(), data.size(), sizeof(T), D3D11_BIND_VERTEX_BUFFER, &dxBuffer);

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

			UINT stride = sizeof(T);
			UINT offset = 0;
			m_device->_context->IASetVertexBuffers(0, 1,
				&dxBuffer, &stride, &offset);

			return true;
		}

	public:		
		ID3D11Buffer* dxBuffer = nullptr;
		std::vector<T> data;		
	};
}

#endif