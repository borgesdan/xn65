#ifndef XNA_GRAPHICS_TEXTURE_HPP
#define XNA_GRAPHICS_TEXTURE_HPP

#include "csharp/io/stream.hpp"
#include "gresource.hpp"
#include "shared.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace xna {
	//Represents a texture resource. 
	class Texture : public GraphicsResource {
	public:
		Texture(std::shared_ptr<GraphicsDevice> const& graphicsDevice) : GraphicsResource(graphicsDevice) {}		

		//Gets the format of the texture data.
		virtual SurfaceFormat Format() const = 0;
		//Gets the number of texture levels in a multilevel texture. 
		virtual int32_t LevelCount() const = 0;
	};

	struct Texture2DImplementation;

	//Represents a 2D grid of texels. 
	class Texture2D : public Texture {
	public:
		Texture2D();
		Texture2D(std::shared_ptr<GraphicsDevice> const& device);
		Texture2D(std::shared_ptr<GraphicsDevice> const& device, size_t width, size_t height);
		Texture2D(std::shared_ptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format);			

		//Gets the width of this texture resource, in pixels.
		constexpr int32_t Width() const { return width; }
		//Gets the height of this texture resource, in pixels.
		constexpr int32_t Height() const { return height; }
		//Gets the size of this resource.
		constexpr Rectangle Bounds() const { return { 0, 0, width, height }; }
		//Gets the format of the texture data.
		constexpr SurfaceFormat Format() const override { return surfaceFormat; }
		//Gets the number of texture levels in a multilevel texture. 
		constexpr int32_t LevelCount() const { return levelCount; }

		//Sets data to the texture.
		void SetData(std::vector<Color> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(std::vector<uint32_t> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(std::vector<uint8_t> const& data, size_t startIndex = 0, size_t elementCount = 0);
		//Sets data to the texture.
		void SetData(int32_t level, Rectangle* rect, std::vector<uint8_t> const& data, size_t startIndex, size_t elementCount);
		
		//Loads texture data from a stream. 
		static std::shared_ptr<Texture2D> FromStream(GraphicsDevice& device, csharp::Stream& stream);
		//Loads texture data from a file. 
		static std::shared_ptr<Texture2D> FromStream(GraphicsDevice& device, std::string const& fileName);
		//Loads texture data from a data. 
		static std::shared_ptr<Texture2D> FromStream(GraphicsDevice& device, std::vector<uint8_t> const& data);	
		
		void Initialize();

		std::unique_ptr<Texture2DImplementation> Implementation;

	protected:
		SurfaceFormat surfaceFormat{ SurfaceFormat::Color };
		int32_t levelCount{ 0 };
		int width{ 0 };
		int height{ 0 };	
	};
}

#endif