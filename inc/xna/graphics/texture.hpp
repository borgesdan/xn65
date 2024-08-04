#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	//Represents a texture resource. 
	class Texture : public GraphicsResource {
	public:
		Texture(P_GraphicsDevice const& graphicsDevice) : GraphicsResource(graphicsDevice) {}		

		//Gets the format of the texture data.
		virtual SurfaceFormat Format() const = 0;
		//Gets the number of texture levels in a multilevel texture. 
		virtual Int LevelCount() const = 0;
	};

	//Represents a 2D grid of texels. 
	class Texture2D : public Texture {
	public:
		Texture2D();
		Texture2D(P_GraphicsDevice const& device);
		Texture2D(P_GraphicsDevice const& device, size_t width, size_t height);
		Texture2D(P_GraphicsDevice const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format);			

		//Gets the width of this texture resource, in pixels.
		constexpr Int Width() const { return width; }
		//Gets the height of this texture resource, in pixels.
		constexpr Int Height() const { return height; }
		//Gets the size of this resource.
		constexpr Rectangle Bounds() const { return { 0, 0, width, height }; }
		//Gets the format of the texture data.
		constexpr SurfaceFormat Format() const override { return surfaceFormat; }
		//Gets the number of texture levels in a multilevel texture. 
		constexpr Int LevelCount() const { return levelCount; }

		//Sets data to the texture.
		void SetData(std::vector<Color> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(std::vector<Uint> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(std::vector<Byte> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount);
		
		//Loads texture data from a stream. 
		static P_Texture2D FromStream(GraphicsDevice& device, P_Stream const& stream);
		//Loads texture data from a file. 
		static P_Texture2D FromStream(GraphicsDevice& device, String const& fileName);
		//Loads texture data from a data. 
		static P_Texture2D FromStream(GraphicsDevice& device, std::vector<Byte> const& data);	
		
		void Initialize();

	protected:
		SurfaceFormat surfaceFormat{ SurfaceFormat::Color };
		Int levelCount{ 0 };
		int width{ 0 };
		int height{ 0 };

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	
	using PTexture2D = P_Texture2D;
}

#endif