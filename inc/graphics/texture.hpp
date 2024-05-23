#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class Texture {
	public:
		~Texture() {}
	};

	class Texture2D : public Texture, public GraphicsResource {
	public:
		Texture2D();
		Texture2D(sptr<GraphicsDevice> const& device);
		Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height);
		Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format);
		~Texture2D() override;
		Int Width() const;
		Int Height() const;
		Rectangle Bounds() const;
		bool Initialize(xna_error_nullarg);
		void SetData(std::vector<Color> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		void SetData(std::vector<Uint> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		void SetData(std::vector<Byte> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		void SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount, xna_error_nullarg);
		static sptr<Texture2D> FromStream(GraphicsDevice& device, String const& fileName, xna_error_nullarg);
		static sptr<Texture2D> FromMemory(GraphicsDevice& device, std::vector<Byte> const& data, xna_error_nullarg);	

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif