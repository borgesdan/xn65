#ifndef XNA_GRAPHICS_DISPLAYMODE_HPP
#define XNA_GRAPHICS_DISPLAYMODE_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"

namespace xna {
	//Flags indicating the method the raster uses to create an image on a surface
	enum class DisplayModeScanlineOrder {
		Unspecified = 0,
		Progressive = 1,
		UpperFieldFirst = 2,
		LowerFieldFirst = 3
	};

	//Flags indicating how an image is stretched to fit a given monitor's resolution
	enum class DisplayModeScaling {
		Unspecified = 0,
		Centered = 1,
		Stretched = 2
	};

	struct DisplayModeRate {
		constexpr DisplayModeRate() = default;

		constexpr DisplayModeRate(DisplayModeScanlineOrder scanlineOrdering, DisplayModeScaling scaling, RationalNumber refreshRate) :
		ScanlineOrdering(scanlineOrdering), Scaling(scaling), RefreshRate(refreshRate){}

		constexpr bool operator==(const DisplayModeRate& other) const {
			return ScanlineOrdering == other.ScanlineOrdering && Scaling == other.Scaling && RefreshRate == other.RefreshRate;
		}

		//Gets the method the raster uses to create an image on a surface
		DisplayModeScanlineOrder ScanlineOrdering{ DisplayModeScanlineOrder::Unspecified };
		//Gets how an image is stretched to fit a given monitor's resolution
		DisplayModeScaling Scaling{ DisplayModeScaling::Unspecified };
		//Describing the refresh rate in hertz.
		RationalNumber RefreshRate{};		
	};

	//Describes the display mode. 
	class DisplayMode {
	public:
		constexpr DisplayMode();	

		constexpr DisplayMode(Int width, Int height, SurfaceFormat format):
			width(width), height(height), format(format){}

		//Gets the aspect ratio used by the graphics device.
		constexpr float AspectRatio() const {
			if (height == 0 || width == 0)
				return 0;

			return static_cast<float>(width) / static_cast<float>(height);
		}		

		//Gets a value indicating the screen width, in pixels. 
		constexpr Int Width() const { return width; }
		//Gets a value indicating the screen height, in pixels. 
		constexpr Int Height() const { return height; }
		//Gets a value indicating the surface format of the display mode.
		constexpr SurfaceFormat Format() const { return format; }

		constexpr bool operator==(const DisplayMode& other) const {
			return width == other.width
				&& height == other.height
				&& format == other.format;
		}

	private:		
		Int width{ 0 };		
		Int height{ 0 };		 
		SurfaceFormat format{ SurfaceFormat::Color };		
		
	public:
		std::vector<DisplayModeRate> Rates;
	};

	//Manipulates a collection of DisplayMode structures. 
	class DisplayModeCollection {
	public:
		constexpr DisplayModeCollection() = default;

		DisplayModeCollection(size_t count) : DisplayModes(count) {}

		DisplayModeCollection(std::vector<sptr<DisplayMode>> const& displayModes) :
			DisplayModes(displayModes) {}

		size_t SurfaceCount(SurfaceFormat format) const;

		std::vector<sptr<DisplayMode>> Query(SurfaceFormat format) const;
		sptr<DisplayMode> Query(SurfaceFormat format, Uint width, Uint height) const;

	public:
		std::vector<sptr<DisplayMode>> DisplayModes;
	};
}

#endif