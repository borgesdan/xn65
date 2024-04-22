#include "displaymode-dx.hpp"

namespace xna {
	std::vector<PDisplayMode> DisplayModeCollection::At(SurfaceFormat format) const
	{
		std::vector<PDisplayMode> modes;
		At(format, modes);
		return modes;
	}

	void DisplayModeCollection::At(SurfaceFormat format, std::vector<PDisplayMode>& modes) const
	{
		size_t counter = 0;

		for (size_t i = 0; i < _displayModes.size(); ++i) {
			const auto& displayMode = _displayModes[i];

			if (displayMode->Format() == format)
			{
				modes.push_back(displayMode);
				++counter;
			}
		}

		if (!modes.empty())
			modes.resize(counter);
	}

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
}