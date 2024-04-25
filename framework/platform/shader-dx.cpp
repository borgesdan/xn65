#include "shader-dx.hpp"
#include "device-dx.hpp"
#include "databuffer-dx.hpp"

namespace xna {
    HRESULT Shader::CompileFromFile(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
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

    bool VertexShader::Initialize(DataBuffer& buffer, xna_error_ptr_arg)
    {
        if (!m_device || !m_device->_device || !buffer._blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        if (_vertexShader) {
            _vertexShader->Release();
            _vertexShader = nullptr;
        }

        const auto hr = m_device->_device->CreateVertexShader(
            buffer._blob->GetBufferPointer(),
            buffer._blob->GetBufferSize(),
            NULL,
            &_vertexShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
            return false;
        }

        return true;
    }

    bool PixelShader::Initialize(DataBuffer& buffer, xna_error_ptr_arg)
    {
        if (!m_device || !m_device->_device || !buffer._blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        if (_pixelShader) {
            _pixelShader->Release();
            _pixelShader = nullptr;
        }

        const auto hr = m_device->_device->CreatePixelShader(
            buffer._blob->GetBufferPointer(),
            buffer._blob->GetBufferSize(),
            NULL,
            &_pixelShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
            return false;
        }

        return true;
    }
}