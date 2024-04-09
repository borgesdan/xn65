#include "shader-dx.hpp"
#include "device-dx.hpp"

namespace xna {
    HRESULT Shader::CompileFromFile(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
	{
		//https://learn.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader

        if (!srcFile || !entryPoint || !profile || !blob)
            return E_INVALIDARG;

        *blob = nullptr;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
        flags |= D3DCOMPILE_DEBUG;
#endif

        const D3D_SHADER_MACRO defines[] =
        {
            "EXAMPLE_DEFINE", "1",
            NULL, NULL
        };

        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint, profile,
            flags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            if (errorBlob)
            {
                OutputDebugStringA((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();

            return hr;
        }

        *blob = shaderBlob;

        return hr;
	}

    bool VertexShader::Initialize(ID3DBlob* blob, xna_error_ptr_arg) {
        if (!_device || !blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        const auto hr = _device->_device->CreateVertexShader(
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            NULL,
            &_vertexShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        return true;
    }

    bool PixelShader::Initialize(ID3DBlob* blob, xna_error_ptr_arg)
    {
        if (!_device || !blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        const auto hr = _device->_device->CreatePixelShader(
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            NULL,
            &_pixelShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        return true;
    }
}