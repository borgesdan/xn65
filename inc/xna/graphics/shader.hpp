#ifndef XNA_GRAPHICS_SHADER_HPP
#define XNA_GRAPHICS_SHADER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class Shader : public GraphicsResource {
	public:
		Shader();
		Shader(sptr<GraphicsDevice> const& device);
		~Shader() override {}
		bool Initialize(DataBuffer& buffer);	
		static bool CompileFromFile(WString srcFile, String entryPoint, String profile, DataBuffer& blob);
	};

	class VertexShader : public Shader {
	public:
		VertexShader();
		VertexShader(sptr<GraphicsDevice> const& device);
		~VertexShader() override;
		bool Initialize(DataBuffer& buffer);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
	
	class PixelShader : public Shader {
	public:
		PixelShader();
		PixelShader(sptr<GraphicsDevice> const& device);
		~PixelShader() override;
		bool Initialize(DataBuffer& buffer);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif