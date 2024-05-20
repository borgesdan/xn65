#ifndef XNA_GRAPHICS_DISPLAYMODE_HPP
#define XNA_GRAPHICS_DISPLAYMODE_HPP

#include "../default.hpp"

namespace xna {
	struct DisplayModeDescription;

	class DisplayMode {
	public:
		DisplayMode();		
		~DisplayMode();

		constexpr float AspectRatio() const {
			if (Height == 0 || Width == 0)
				return 0;

			return static_cast<float>(Width) / static_cast<float>(Height);
		}		

		constexpr bool operator==(const DisplayMode& other) const {
			return Width == other.Width
				&& Height == other.Height
				&& Format == other.Format;
		}

	public:
		Int Width{ 0 };
		Int Height{ 0 };
		SurfaceFormat Format{ SurfaceFormat::Color };		

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;
	};

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