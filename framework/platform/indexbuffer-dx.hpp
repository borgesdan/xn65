#ifndef XNA_PLATFORM_INDEXBUFFER_DX_HPP
#define XNA_PLATFORM_INDEXBUFFER_DX_HPP

#include "../graphics/indexbuffer.hpp"
#include "dxheaders.hpp"

namespace xna {
	class IndexBuffer : public IIndexBuffer {
	public:
		IndexBuffer() {			
			_description.Usage = D3D11_USAGE_DEFAULT;
			_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
			
		}
		
		IndexBuffer(size_t size) {
			_description.ByteWidth = static_cast<UINT>(size);
			_description.Usage = D3D11_USAGE_DEFAULT;
			_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
		}

		IndexBuffer(D3D11_BUFFER_DESC desc) : _description(desc){}

		virtual ~IndexBuffer() override {
			if (_buffer) {
				_buffer->Release();
				_buffer = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) override;

	public:
		D3D11_BUFFER_DESC _description;
		ID3D11Buffer* _buffer = nullptr;
		D3D11_SUBRESOURCE_DATA _initialData{}
	};
}

#endif