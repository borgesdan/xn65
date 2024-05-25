#include "graphics/buffer.hpp"
#include "platform-dx/implementations.hpp"
#include "graphics/shader.hpp"

namespace xna {
    static HRESULT shaderCompileFromFile(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
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

    bool Shader::CompileFromFile(WString srcFile, String entryPoint, String profile, DataBuffer& blob) {
        const auto hr = shaderCompileFromFile(srcFile.c_str(), entryPoint.c_str(), profile.c_str(), &blob.impl->_blob);

        return SUCCEEDED(hr);
    }

    VertexShader::~VertexShader() {
        impl = nullptr;
    }

    PixelShader::~PixelShader() {
        impl = nullptr;
    }

    bool VertexShader::Initialize(DataBuffer& buffer, xna_error_ptr_arg)
    {
        if (!impl || !m_device || !m_device->impl->_device || !buffer.impl->_blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        if (impl->_vertexShader) {
            impl->_vertexShader->Release();
            impl->_vertexShader = nullptr;
        }

        const auto hr = m_device->impl->_device->CreateVertexShader(
            buffer.impl->_blob->GetBufferPointer(),
            buffer.impl->_blob->GetBufferSize(),
            NULL,
            &impl->_vertexShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
            return false;
        }

        return true;
    }

    bool PixelShader::Initialize(DataBuffer& buffer, xna_error_ptr_arg)
    {
        if (!impl || !m_device || !m_device->impl->_device || !buffer.impl->_blob) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        if (impl->_pixelShader) {
            impl->_pixelShader->Release();
            impl->_pixelShader = nullptr;
        }

        const auto hr = m_device->impl->_device->CreatePixelShader(
            buffer.impl->_blob->GetBufferPointer(),
            buffer.impl->_blob->GetBufferSize(),
            NULL,
            &impl->_pixelShader);

        if (FAILED(hr)) {
            xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
            return false;
        }

        return true;
    }
}