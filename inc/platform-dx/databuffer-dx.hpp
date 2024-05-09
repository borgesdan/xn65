#ifndef XNA_PLATFORM_DATABUFFER_DX_HPP
#define XNA_PLATFORM_DATABUFFER_DX_HPP

#include "../graphics/databuffer.hpp"
#include "dxheaders.hpp"

namespace xna {
	class DataBuffer : public IDataBuffer {
	public:
		virtual ~DataBuffer() override {
			if (_blob) {
				_blob->Release();
				_blob = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) override {
			return false;
		}

	public:
		ID3DBlob* _blob = nullptr;
	};
}

#endif