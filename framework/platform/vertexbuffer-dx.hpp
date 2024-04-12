#ifndef XNA_PLATFORM_VERTEXBUFFER_DX_HPP
#define XNA_PLATFORM_VERTEXBUFFER_DX_HPP

#include "../graphics/vertexbuffer.hpp"
#include "dxheaders.hpp"
#include "../graphics/vertexposition.hpp"

namespace xna {
	class VertexBuffer : public IVertexBuffer {
	public:
		VertexBuffer() {
			_description.Usage = D3D11_USAGE_DYNAMIC;
			_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		VertexBuffer(size_t size) {
			_description.ByteWidth = static_cast<UINT>(size);
			_description.Usage = D3D11_USAGE_DYNAMIC;
			_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		VertexBuffer(D3D11_BUFFER_DESC desc) : _description(desc){}

		virtual ~VertexBuffer() override {
			if (_buffer) {
				_buffer->Release();
				_buffer = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) override;

	public:
		D3D11_BUFFER_DESC _description{};
		D3D11_SUBRESOURCE_DATA _subResource{};
		ID3D11Buffer* _buffer = nullptr;
	};
}

#endif