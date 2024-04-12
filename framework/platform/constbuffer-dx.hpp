#ifndef XNA_PLATFORM_CONSTBUFFER_DX_HPP
#define XNA_PLATFORM_CONSTBUFFER_DX_HPP

#include "../graphics/constbuffer.hpp"
#include "../common/matrix.hpp"
#include "dxheaders.hpp"

namespace xna {
	class ConstantBuffer : public IConstantBuffer {
	public:
		virtual ~ConstantBuffer() override {
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
		DirectX::XMMATRIX _worldViewProjection;
	};
}

#endif