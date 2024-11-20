#ifndef XNA_PIPELINE_GRAPHICS_HPP
#define XNA_PIPELINE_GRAPHICS_HPP

#include "pipeline.hpp"
#include <vector>
#include <cstdint>
#include "xna/exception.hpp"
#include "xna/common/numerics.hpp"
#include "xna/graphics/shared.hpp"

namespace xna {
	class BitmapContent : public ContentItem {
	public:		
		BitmapContent(int32_t width, int32_t height) {
			if (width <= 0 || height <= 0)
				Exception::Throw(Exception::INVALID_OPERATION);

			this->width = width;
			this->height = height;
		}

		//Gets or sets the width of the bitmap, in pixels.
		constexpr int32_t Width() const { return width; }
		
		//Gets or sets the width of the bitmap, in pixels.
		constexpr void Width(int32_t value) {
			if(value <= 0)
				Exception::Throw(Exception::INVALID_OPERATION);

			width = value;
		}

		//Gets or sets the height of the bitmap, in pixels.
		constexpr int32_t Height() const { return height; }

		//Gets or sets the height of the bitmap, in pixels.
		constexpr void Height(int32_t value) {
			if (value <= 0)
				Exception::Throw(Exception::INVALID_OPERATION);

			height = value;
		}

		//Copies one bitmap into another.
		static inline void Copy(BitmapContent const& sourceBitmap, BitmapContent& destinationBitmap) {
			BitmapContent::Copy(sourceBitmap, Rectangle(0, 0, sourceBitmap.Width(), sourceBitmap.Height()),
				destinationBitmap, Rectangle(0, 0, destinationBitmap.Width(), destinationBitmap.Height()));
		}

		//Copies one bitmap into another.
		static void Copy(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion,
			BitmapContent& destinationBitmap, Rectangle const& destinationRegion);

		static bool InteropCopy(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion,
			BitmapContent& destinationBitmap, Rectangle const& destinationRegion);

		//Writes encoded bitmap content.
		virtual void SetPixelData(std::vector<uint8_t> const& sourceData) = 0;
		//Reads encoded bitmap content.
		virtual std::vector<uint8_t> GetPixelData() const = 0;
		//Gets the corresponding GPU texture format for the specified bitmap type.
		virtual bool TryGetFormat(SurfaceFormat& format) const = 0;

	protected:
		BitmapContent() = default;

		//Validates the arguments to the Copy function.
		static void ValidateCopyArguments(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion,
			BitmapContent const& destinationBitmap, Rectangle const& destinationRegion);		

		//Attempts to copy a region of the specified bitmap onto another.
		virtual bool TryCopyTo(BitmapContent& destinationBitmap, Rectangle const& sourceRegion, Rectangle const& destionationRegion) const = 0;

		//Attempts to copy a region from a specified bitmap.
		virtual bool TryCopyFrom(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion, Rectangle const& destionationRegion) = 0;

	private:
		int32_t width{ 0 };
		int32_t height{ 0 };
	};

	template <typename T>
	class PixelBitmapContent : public BitmapContent {
	public:
		PixelBitmapContent(int32_t width, int32_t height) : BitmapContent(width, height){}

		//Writes encoded bitmap content.
		void SetPixelData(std::vector<uint8_t> const& sourceData) override {}
		//Reads encoded bitmap content.
		std::vector<uint8_t> GetPixelData() const override { return {}; }
		//Gets the corresponding GPU texture format for the specified bitmap type.
		bool TryGetFormat(SurfaceFormat& format) const override { return false; }

	protected:
		//Attempts to copy a region of the specified bitmap onto another.
		bool TryCopyTo(BitmapContent& destinationBitmap, Rectangle const& sourceRegion, Rectangle const& destionationRegion) const override { return false; }

		//Attempts to copy a region from a specified bitmap.
		bool TryCopyFrom(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion, Rectangle const& destionationRegion) override { return false; }
	};


	//Provides methods for accessing a mipmap chain.
	class MipmapChain {

	};

	//Provides methods for maintaining a mipmap chain.
	class MipmapChainCollection {
	public:

	private:

	};

	//Provides a base class for all texture objects.
	class TextureContent : public ContentItem {
	};
}

#endif