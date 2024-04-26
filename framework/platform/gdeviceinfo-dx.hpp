#ifndef XNA_PLATFORM_GDEVICEINFOR_DX_HPP
#define XNA_PLATFORM_GDEVICEINFOR_DX_HPP

#include "../game/gdeviceinfo.hpp"
#include "adapter-dx.hpp"
#include "window-dx.hpp"
#include "presentparameters-dx.hpp"

namespace xna {
	class GraphicsDeviceInformation : public IGraphicsDeviceInformation {
	public:
		virtual ~GraphicsDeviceInformation() override {}

		inline virtual PGraphicsAdapter Adapter() const override {
			return _adapter;
		};

		inline virtual void Adapter(PGraphicsAdapter const& value) override {
			_adapter = value;
		}

		constexpr virtual xna::PresentationParameters PresentationParameters() const override{
			return _parameters;
		};

		constexpr virtual void PresentationParameters(xna::PresentationParameters const& value) override{
			_parameters = value;
		};

		constexpr virtual xna::GraphicsProfile GraphicsProfile() const override {
			return _profile;
		};

		constexpr virtual void GraphicsProfile(xna::GraphicsProfile value) override {
			_profile = value;
		};

		inline virtual PGameWindow Window() const override {
			return _window;
		}

		inline virtual void Window(PGameWindow const& window) override {
			_window = window;
		}

	public:
		PGraphicsAdapter _adapter{ nullptr };
		xna::GraphicsProfile _profile{xna::GraphicsProfile::Reach};
		xna::PresentationParameters _parameters{};
		PGameWindow _window{ nullptr };
	};
}

#endif