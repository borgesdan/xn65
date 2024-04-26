#include "displaymode-dx.hpp"

namespace xna {
	size_t DisplayModeCollection::SurfaceCount(SurfaceFormat format) const
	{
		size_t counter = 0;

		for (size_t i = 0; i < _displayModes.size(); ++i) {
			if (_displayModes[i]->Format() == format) {
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

		for (size_t i = 0; i < _displayModes.size(); ++i) {
			if (_displayModes[i]->Format() == format) {
				modes[index] = _displayModes[i];
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

		for (size_t i = 0; i < _displayModes.size(); ++i) {
			const auto& mode = _displayModes[i];

			if (mode->Format() == format && mode->_width == width && mode->_height == height) {
				return _displayModes[i];
			}
			else if(mode->Format() == format && mode->_width == width) {
				matched = mode;
			}
		}

		return matched;
	}

}