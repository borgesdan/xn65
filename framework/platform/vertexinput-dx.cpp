#include "vertexinput-dx.hpp"
#include "device-dx.hpp"
#include <d3dcompiler.h>
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	bool VertexInputLayout::Initialize(ID3DBlob* blob, xna_error_ptr_arg){
		if (!_device || !blob) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		const auto hr = _device->_device->CreateInputLayout(
			_description.data(),
			_description.size(),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&_inputLayout);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		return true;
	}    
}