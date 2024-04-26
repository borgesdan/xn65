#ifndef XNA_GAME_GDEVICEINFO_HPP
#define XNA_GAME_GDEVICEINFO_HPP

#include "../forward.hpp"
#include "../enums.hpp"

namespace xna {
	class IGraphicsDeviceInformation {
	public:
		virtual ~IGraphicsDeviceInformation(){}
		virtual sptr<GraphicsAdapter> Adapter() const = 0;
		virtual void Adapter(sptr<GraphicsAdapter> const& value) = 0;
		virtual xna::PresentationParameters PresentationParameters() const = 0;
		virtual void PresentationParameters(xna::PresentationParameters const& value) = 0;
		virtual xna::GraphicsProfile GraphicsProfile() const = 0;
		virtual void GraphicsProfile(xna::GraphicsProfile value) = 0;
		virtual sptr<GameWindow> Window() const = 0;
		virtual void Window(sptr<GameWindow> const& window) = 0;
	};
}

#endif