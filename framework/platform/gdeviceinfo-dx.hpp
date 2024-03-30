#ifndef XNA_PLATFORM_GDEVICEINFOR_DX_HPP
#define XNA_PLATFORM_GDEVICEINFOR_DX_HPP

#include "../game/gdeviceinfo.hpp"
#include "adapter-dx.hpp"

namespace xna {
	class GraphicsDeviceInformation : public IGraphicsDeviceInformation {
	public:
		inline virtual PGraphicsAdapter Adapter() const override {
			return _adapter;
		};

		inline virtual void Adapter(PGraphicsAdapter const& value) override {
			_adapter = value;
		}

		inline virtual xna::PresentationParameters PresentationParameters() const override{
			return _parameters;
		};

		inline virtual void PresentationParameters(xna::PresentationParameters const& value) override{
			_parameters = value;
		};

		inline virtual xna::GraphicsProfile GraphicsProfile() const override {
			return _profile;
		};

		inline virtual void GraphicsProfile(xna::GraphicsProfile value) override {
			_profile = value;
		};

	private:
		PGraphicsAdapter _adapter{ nullptr };
		xna::GraphicsProfile _profile{xna::GraphicsProfile::Reach};
		xna::PresentationParameters _parameters{};
	};
}

#endif