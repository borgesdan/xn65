#include "vertexinput-dx.hpp"
#include "device-dx.hpp"
#include <d3dcompiler.h>
#include "dxgi.h"
#include "d3d11.h"
#include "databuffer-dx.hpp"

namespace xna {
	bool VertexInputLayout::Initialize(GraphicsDevice& device, DataBuffer& blob, xna_error_ptr_arg){
		if (!device._device || !blob._blob) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (_inputLayout) {
			_inputLayout->Release();
			_inputLayout = nullptr;
		}

		const auto hr = device._device->CreateInputLayout(
			_description.data(),
			static_cast<UINT>(_description.size()),
			blob._blob->GetBufferPointer(),
			blob._blob->GetBufferSize(),
			&_inputLayout);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}    
}