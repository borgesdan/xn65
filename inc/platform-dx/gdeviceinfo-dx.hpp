#ifndef XNA_PLATFORM_GDEVICEINFOR_DX_HPP
#define XNA_PLATFORM_GDEVICEINFOR_DX_HPP

#include "../game/gdeviceinfo.hpp"
#include "window-dx.hpp"
#include "graphics/presentparams.hpp"

namespace xna {
	class GraphicsDeviceInformation : public IGraphicsDeviceInformation {
	public:
		virtual ~GraphicsDeviceInformation() override {}

		inline virtual sptr<GraphicsAdapter> Adapter() const override {
			return _adapter;
		};

		inline virtual void Adapter(sptr<GraphicsAdapter> const& value) override {
			_adapter = value;
		}

		virtual xna::PresentationParameters PresentationParameters() const override{
			return _parameters;
		};

		virtual void PresentationParameters(xna::PresentationParameters const& value) override{
			_parameters = value;
		};

		virtual xna::GraphicsProfile GraphicsProfile() const override {
			return _profile;
		};

		virtual void GraphicsProfile(xna::GraphicsProfile value) override {
			_profile = value;
		};

		inline virtual sptr<GameWindow> Window() const override {
			return _window;
		}

		inline virtual void Window(sptr<GameWindow> const& window) override {
			_window = window;
		}

	public:
		sptr<GraphicsAdapter> _adapter{ nullptr };
		xna::GraphicsProfile _profile{xna::GraphicsProfile::Reach};
		xna::PresentationParameters _parameters{};
		sptr<GameWindow> _window{ nullptr };
	};
}

#endif