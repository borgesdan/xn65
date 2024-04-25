#ifndef XNA_PLATFORM_INDEXBUFFER_DX_HPP
#define XNA_PLATFORM_INDEXBUFFER_DX_HPP

#include "../graphics/indexbuffer.hpp"
#include "device-dx.hpp"
#include "dxheaders.hpp"
#include <BufferHelpers.h>
#include <VertexTypes.h>

namespace xna {
	template <typename T>
	class IndexBuffer : public IIndexBuffer {
	public:
		constexpr IndexBuffer() = default;

		constexpr IndexBuffer(std::vector<T> const& vertices) : data(vertices) {}

		virtual ~IndexBuffer() override {
			if (dxBuffer) {
				dxBuffer->Release();
				dxBuffer = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) override {
			if (!device._device) {
				xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
				return false;
			}

			if (data.empty()) {
				xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
				return false;
			}

			const auto hr = DirectX::CreateStaticBuffer(device._device, data.data(), data.size(), sizeof(T), D3D11_BIND_INDEX_BUFFER, &dxBuffer);

			if (FAILED(hr)) {
				xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
				return false;
			}

			return true;
		}

	public:
		ID3D11Buffer* dxBuffer = nullptr;
		std::vector<T> data;
	};
}

#endif