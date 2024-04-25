#ifndef XNA_PLATFORM_SHADER_DX_HPP
#define XNA_PLATFORM_SHADER_DX_HPP

#include "../graphics/shader.hpp"
#include "../graphics/gresource.hpp"
#include "dxheaders.hpp"

namespace xna {
	class Shader : public IShader, public GraphicsResource {
	public:
		Shader(GraphicsDevice* device) : GraphicsResource(device){}

		virtual ~Shader() override {}

		virtual bool Initialize(DataBuffer& buffer, xna_error_nullarg) override {}

		static HRESULT CompileFromFile(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);			
	};	

	class VertexShader : public Shader {
	public:
		VertexShader(GraphicsDevice* device) : Shader(device){}

		virtual ~VertexShader() override {
			if (_vertexShader) {
				_vertexShader->Release();
				_vertexShader = nullptr;
			}
		}

		virtual bool Initialize(DataBuffer& buffer, xna_error_nullarg) override;

	public:
		ID3D11VertexShader* _vertexShader = nullptr;
	};

	class PixelShader : public Shader {
	public:
		PixelShader(GraphicsDevice* device) : Shader(device) {}

		virtual ~PixelShader() override {
			if (_pixelShader) {
				_pixelShader->Release();
				_pixelShader = nullptr;
			}
		}

		virtual bool Initialize(DataBuffer& buffer, xna_error_nullarg) override;

	public:
		ID3D11PixelShader* _pixelShader = nullptr;
	};
}

#endif