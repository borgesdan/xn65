#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../enums.hpp"
#include "../types.hpp"
#include "../forward.hpp"
#include "displaymode.hpp"

namespace xna {
	class IGraphicsAdapter {
	public:
		virtual ~IGraphicsAdapter() {}					

		virtual String Description() const = 0;
		virtual Uint DeviceId() const = 0;
		virtual String DeviceName() const = 0;
		virtual bool IsDefaultAdapter() const = 0;
		virtual intptr_t MonitorHandle() const = 0;
		virtual Uint Revision() const = 0;
		virtual Uint SubSystemId() const = 0;
		virtual Uint VendorId() const = 0;
		virtual UDisplayModeCollection SupportedDisplayModes() const = 0;

		static UGraphicsAdapter DefaultAdapter();
		static void GetAllAdapters(std::vector<PGraphicsAdapter>& adapters);
		static void GetAllAdapters(std::vector<UGraphicsAdapter>& adapters);
	};
}

#endif