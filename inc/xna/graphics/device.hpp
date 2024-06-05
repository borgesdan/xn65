#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "../default.hpp"

namespace xna {
	class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice> {
	public:
		GraphicsDevice();
		GraphicsDevice(GraphicsDeviceInformation const& info);
		~GraphicsDevice();
		void Clear();
		void Clear(Color const& color);
		bool Initialize();
		bool Present();
		sptr<GraphicsAdapter> Adapter() const;
		void Adapter(sptr<GraphicsAdapter> const& adapter);
		xna::Viewport Viewport() const;
		void Viewport(xna::Viewport const& viewport);
		void UseVSync(bool use);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif