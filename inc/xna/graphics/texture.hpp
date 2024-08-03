#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	//Represents a texture resource. 
	class Texture : public GraphicsResource {
	public:
		Texture(P_GraphicsDevice const& graphicsDevice) : GraphicsResource(graphicsDevice) {}

		virtual ~Texture() {}

		//Gets the format of the texture data.
		constexpr SurfaceFormat Format() const { return surfaceFormat; }
		//Gets the number of texture levels in a multilevel texture. 
		constexpr Int LevelCount() const { return levelCount; }

	protected:
		SurfaceFormat surfaceFormat{SurfaceFormat::Color};
		Int levelCount{ 0 };
	};

	class Texture2D : public Texture {
	public:
		Texture2D();
		Texture2D(P_GraphicsDevice const& device);
		Texture2D(P_GraphicsDevice const& device, size_t width, size_t height);
		Texture2D(P_GraphicsDevice const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format);
		~Texture2D() override;
		Int Width() const;
		Int Height() const;
		Rectangle Bounds() const;
		void SetData(std::vector<Color> const& data, size_t startIndex = 0, size_t elementCount = 0);
		void SetData(std::vector<Uint> const& data, size_t startIndex = 0, size_t elementCount = 0);
		void SetData(std::vector<Byte> const& data, size_t startIndex = 0, size_t elementCount = 0);
		void SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount);
		static P_Texture2D FromStream(GraphicsDevice& device, String const& fileName);
		static P_Texture2D FromMemory(GraphicsDevice& device, std::vector<Byte> const& data);	
		
	void Initialize();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	
	using PTexture2D = P_Texture2D;
}

#endif