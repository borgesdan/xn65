#ifndef XNA_GRAPHICS_DISPLAYMODE_HPP
#define XNA_GRAPHICS_DISPLAYMODE_HPP

#include "../enums.hpp"
#include "../types.hpp"
#include "../forward.hpp"

namespace xna {
	class DisplayMode {
	public:
		constexpr DisplayMode() = default;

		constexpr DisplayMode(Int width, Int height, SurfaceFormat format)
			: _width(width), _height(height), _format(format) {}

		constexpr float AspectRatio() const noexcept {
			if (_height == 0 || _width == 0)
				return 0;

			return static_cast<float>(_width) / static_cast<float>(_height);
		}

		constexpr Int Width() const {
			return _width;
		}

		constexpr Int Height() const {
			return _height;
		}

		constexpr SurfaceFormat Format() const {
			return _format;
		}

	public:
		Int _width{ 0 };
		Int _height{ 0 };
		SurfaceFormat _format{ SurfaceFormat::Color };
	};	

	class DisplayModeCollection {
	public:
		constexpr DisplayModeCollection() = default;

		DisplayModeCollection(size_t count) : _displayModes(count){}

		DisplayModeCollection(std::vector<DisplayMode> const& displayModes) :
			_displayModes(displayModes) {}

		std::vector<DisplayMode> At(SurfaceFormat format) const {
			std::vector<DisplayMode> modes;
			At(format, modes);
			return modes;
		}		

		void At(SurfaceFormat format, std::vector<DisplayMode>& modes) const {
			size_t counter = 0;

			for (size_t i = 0; i < _displayModes.size(); ++i) {
				const auto& displayMode = _displayModes[i];

				if (displayMode.Format() == format)
				{
					modes.push_back(displayMode);
					++counter;
				}
			}

			if (!modes.empty())
				modes.resize(counter);			
		}

		size_t SurfaceCount(SurfaceFormat format) const {
			size_t counter = 0;

			for (size_t i = 0; i < _displayModes.size(); ++i) {
				if (_displayModes[i].Format() == format) {
					++counter;
				}
			}

			return counter;
		}

		std::vector<DisplayMode> operator[](SurfaceFormat format) const {
			return At(format);
		}

	public:
		std::vector<DisplayMode> _displayModes;
	};
}

#endif