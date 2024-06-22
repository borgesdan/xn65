#ifndef XNA_GRAPHICS_BUFFER_HPP
#define XNA_GRAPHICS_BUFFER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class ConstantBuffer : public GraphicsResource {
	public:
		ConstantBuffer();
		ConstantBuffer(sptr<GraphicsDevice> const&);
		~ConstantBuffer() override;
		bool Initialize();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	class DataBuffer : public GraphicsResource {
	public:
		DataBuffer();
		DataBuffer(sptr<GraphicsDevice> const&);
		~DataBuffer() override;
		bool Initialize();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};	
	
	class IndexBuffer : public GraphicsResource {
	public:
		IndexBuffer();
		IndexBuffer(sptr<GraphicsDevice> const&);
		~IndexBuffer() override;

		template <typename T>
		bool Initialize(std::vector<T> const& data);	
		bool Apply();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	class VertexBuffer : public GraphicsResource {
	public:
		VertexBuffer();
		VertexBuffer(sptr<GraphicsDevice> const&);
		~VertexBuffer();
		template <typename T>
		bool Initialize(std::vector<T> const& data);
		bool Apply();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	class VertexInputLayout : public GraphicsResource {
	public:
		VertexInputLayout();
		VertexInputLayout(sptr<GraphicsDevice> const&);		
		~VertexInputLayout();

		bool Initialize(DataBuffer& blob);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;		
	};
}

#endif