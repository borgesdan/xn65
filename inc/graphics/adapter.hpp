#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../default.hpp"

namespace xna {
	class GraphicsAdapter {
	public:
		GraphicsAdapter();
		~GraphicsAdapter();

		String Description() const;
		Uint DeviceId() const;
		String DeviceName() const;
		bool IsDefaultAdapter() const;
		intptr_t MonitorHandle() const;
		Uint Revision() const;
		Uint SubSystemId() const;
		Uint VendorId() const;
		uptr<DisplayModeCollection> SupportedDisplayModes() const;
		uptr<DisplayModeCollection> SupportedDisplayModes(SurfaceFormat surfaceFormat) const;
		sptr<DisplayMode> CurrentDisplayMode();
		void CurrentDisplayMode(SurfaceFormat surfaceFormat, Uint width, Uint height);

		static uptr<GraphicsAdapter> DefaultAdapter();
		static void Adapters(std::vector<sptr<GraphicsAdapter>>& adapters);
		static void Adapters(std::vector<uptr<GraphicsAdapter>>& adapters);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif