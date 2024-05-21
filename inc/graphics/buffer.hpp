#ifndef XNA_GRAPHICS_BUFFER_HPP
#define XNA_GRAPHICS_BUFFER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class ConstantBuffer : public GraphicsResource {
	public:
		ConstantBuffer();
		ConstantBuffer(sptr<GraphicsDevice> const&);
		~ConstantBuffer();
		bool Initialize(xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	class DataBuffer : public GraphicsResource {
	public:
		DataBuffer();
		DataBuffer(sptr<GraphicsDevice> const&);
		~DataBuffer();
		bool Initialize(xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};	
	
	class IndexBuffer : public GraphicsResource {
	public:
		IndexBuffer();
		IndexBuffer(sptr<GraphicsDevice> const&);
		~IndexBuffer();

		template <typename T>
		bool Initialize(std::vector<T> const& data, xna_error_nullarg);	
		bool Apply(xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif