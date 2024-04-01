#ifndef XNA_GAME_GDEVICEINFO_HPP
#define XNA_GAME_GDEVICEINFO_HPP

#include "../forward.hpp"
#include "../enums.hpp"

namespace xna {
	class IGraphicsDeviceInformation {
	public:
		virtual PGraphicsAdapter Adapter() const = 0;
		virtual void Adapter(PGraphicsAdapter const& value) = 0;
		virtual xna::PresentationParameters PresentationParameters() const = 0;
		virtual void PresentationParameters(xna::PresentationParameters const& value) = 0;
		virtual xna::GraphicsProfile GraphicsProfile() const = 0;
		virtual void GraphicsProfile(xna::GraphicsProfile value) = 0;
		virtual PGameWindow Window() const = 0;
		virtual void Window(PGameWindow const& window) = 0;
	};
}

#endif