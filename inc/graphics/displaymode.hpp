#ifndef XNA_GRAPHICS_DISPLAYMODE_HPP
#define XNA_GRAPHICS_DISPLAYMODE_HPP

#include "../default.hpp"

namespace xna {
	struct DisplayModeDescription;

	class IDisplayMode {
	public:
		virtual ~IDisplayMode() {}
		virtual float AspectRatio() const = 0;
		virtual Int Width() const = 0;
		virtual Int Height() const = 0;
		virtual SurfaceFormat Format() const = 0;
		virtual std::vector<DisplayModeDescription> Descriptions() const = 0;
	};		

	class IDisplayModeCollection {
	public:
		virtual ~IDisplayModeCollection() {}
		virtual size_t SurfaceCount(SurfaceFormat format) const = 0;
	};
}

#endif