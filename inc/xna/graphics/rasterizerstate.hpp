#ifndef XNA_GRAPHICS_RASTERIZER_HPP
#define XNA_GRAPHICS_RASTERIZER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class RasterizerState : GraphicsResource {
	public:
		RasterizerState();
		RasterizerState(sptr<GraphicsDevice> const& device);
		~RasterizerState() override;
		bool Initialize();
		bool Apply();
		xna::CullMode CullMode() const;
		void CullMode(xna::CullMode value);
		xna::FillMode FillMode() const;
		void FillMode(xna::FillMode value);

		static uptr<RasterizerState> CullNone();
		static uptr<RasterizerState> CullClockwise();
		static uptr<RasterizerState> CullCounterClockwise();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif