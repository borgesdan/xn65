#include "indexbuffer-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool IndexBuffer::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._device) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (_buffer) {
			_buffer->Release();
			_buffer = nullptr;
		}

		//TODO: refatorar isso aqui
		int IndicesPerSprite = 4;
		int MaxBatchSize = 4096;
		int VerticesPerSprite = 4;

		std::vector<short> indices;
		indices.reserve(MaxBatchSize * IndicesPerSprite);

		for (short i = 0; i < (MaxBatchSize * VerticesPerSprite); i += VerticesPerSprite)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 2);

			indices.push_back(i + 1);
			indices.push_back(i + 3);
			indices.push_back(i + 2);
		}
		
		_subResource.pSysMem = &indices.front();

		const auto hr = device._device->CreateBuffer(&_description, &_subResource, &_buffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}
}