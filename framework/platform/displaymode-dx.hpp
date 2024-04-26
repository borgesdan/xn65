#ifndef XNA_PLATFORM_DISPLAYMODE_DX_HPP
#define XNA_PLATFORM_DISPLAYMODE_DX_HPP

#include "../graphics/displaymode.hpp"
#include "dxheaders.hpp"

namespace xna {
	struct DisplayModeRefreshRate {
		constexpr DisplayModeRefreshRate() = default;
		
		constexpr DisplayModeRefreshRate(DXGI_RATIONAL const& dxrational) {
			Numerator = dxrational.Numerator;
			Denominator = dxrational.Denominator;
		}
		constexpr DisplayModeRefreshRate(Uint numerator, Uint denominator) 
			: Numerator(numerator), Denominator(denominator){}

		Uint Numerator{ 0 };
		Uint Denominator{ 0 };

		constexpr bool operator==(const DisplayModeRefreshRate& other) const
		{
			return Numerator == other.Numerator && Denominator == other.Denominator;
		}
	};

	struct DisplayModeDescription {
		DisplayModeScanlineOrder _scanlineOrdering{ DisplayModeScanlineOrder::Unspecified };
		DisplayModeScaling _scaling{ DisplayModeScaling::Unspecified };
		DisplayModeRefreshRate _refreshRate{};

		constexpr bool operator==(const DisplayModeDescription& other) const
		{
			return _scanlineOrdering == other._scanlineOrdering && _scaling == other._scaling && _refreshRate == other._refreshRate;
		}
	};

	class DisplayMode : public IDisplayMode {
	public:
		constexpr DisplayMode() = default;

		constexpr DisplayMode(Int width, Int height, SurfaceFormat format)
			: _width(width), _height(height), _format(format) {}

		virtual constexpr float AspectRatio() const noexcept override {
			if (_height == 0 || _width == 0)
				return 0;

			return static_cast<float>(_width) / static_cast<float>(_height);
		}

		virtual constexpr Int Width() const override {
			return _width;
		}

		virtual constexpr Int Height() const override {
			return _height;
		}

		virtual constexpr SurfaceFormat Format() const override {
			return _format;
		}		

		virtual constexpr std::vector<DisplayModeDescription> Descriptions() const {
			return _descriptions;
		}

	public:
		Int _width{ 0 };
		Int _height{ 0 };
		SurfaceFormat _format{ SurfaceFormat::Color };		
		std::vector<DisplayModeDescription> _descriptions;

		constexpr bool operator==(const DisplayMode& other) const {
			return _width == other._width
				&& _height == other._height
				&& _format == other._format
				&& _descriptions == other._descriptions;
		}
	};

	class DisplayModeCollection : public IDisplayModeCollection {
	public:
		constexpr DisplayModeCollection() = default;

		DisplayModeCollection(size_t count) : _displayModes(count) {}

		DisplayModeCollection(std::vector<sptr<DisplayMode>> const& displayModes) :
			_displayModes(displayModes) {}

		virtual size_t SurfaceCount(SurfaceFormat format) const override;

		std::vector<sptr<DisplayMode>> Query(SurfaceFormat format) const;
		sptr<DisplayMode> Query(SurfaceFormat format, Uint width, Uint height) const;

	public:
		std::vector<sptr<DisplayMode>> _displayModes;
	};
}

#endif