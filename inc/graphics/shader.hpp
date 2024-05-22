#ifndef XNA_GRAPHICS_SHADER_HPP
#define XNA_GRAPHICS_SHADER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class Shader : public GraphicsResource {
	public:
		Shader();
		Shader(sptr<GraphicsDevice> const& device);
		~Shader();
		bool Initialize(DataBuffer& buffer, xna_error_nullarg);	
		static bool CompileFromFile(WString srcFile, String entryPoint, String profile, DataBuffer& blob);
	};

	class VertexShader : public Shader {
	public:
		VertexShader();
		VertexShader(sptr<GraphicsDevice> const& device);
		~VertexShader();
		bool Initialize(DataBuffer& buffer, xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
	
	class PixelShader : public Shader {
	public:
		PixelShader();
		PixelShader(sptr<GraphicsDevice> const& device);
		~PixelShader();
		bool Initialize(DataBuffer& buffer, xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif