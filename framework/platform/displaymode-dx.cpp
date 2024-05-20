#include "platform-dx/implementations.hpp"
#include "graphics/displaymode.hpp"

namespace xna {
	DisplayMode::DisplayMode() {
		impl = uNew<PlatformImplementation>();
	}

	DisplayMode::~DisplayMode() {
		impl = nullptr;
	}

	size_t DisplayModeCollection::SurfaceCount(SurfaceFormat format) const
	{
		size_t counter = 0;

		for (size_t i = 0; i < DisplayModes.size(); ++i) {
			if (DisplayModes[i]->Format == format) {
				++counter;
			}
		}

		return counter;
	}

	std::vector<sptr<DisplayMode>> DisplayModeCollection::Query(SurfaceFormat format) const
	{
		const auto count = SurfaceCount(format);
		
		size_t index = 0;
		std::vector<sptr<DisplayMode>> modes(count);

		for (size_t i = 0; i < DisplayModes.size(); ++i) {
			if (DisplayModes[i]->Format == format) {
				modes[index] = DisplayModes[i];
				++index;
			}

			if (index == count)
				break;
		}

		return modes;
	}

	sptr<DisplayMode> DisplayModeCollection::Query(SurfaceFormat format, Uint width, Uint height) const
	{
		sptr<DisplayMode> matched = nullptr;

		for (size_t i = 0; i < DisplayModes.size(); ++i) {
			const auto& mode = DisplayModes[i];

			if (mode->Format == format && mode->Width == width && mode->Height == height) {
				return DisplayModes[i];
			}
			else if(mode->Format == format && mode->Width == width) {
				matched = mode;
			}
		}

		return matched;
	}

}