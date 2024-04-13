#ifndef XNA_GRAPHICS_RASTERIZER_HPP
#define XNA_GRAPHICS_RASTERIZER_HPP

#include "../default.hpp"

namespace xna {
	class IRasterizerState {
	public:
		virtual ~IRasterizerState() {}
		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) = 0;
		virtual xna::CullMode CullMode() const = 0;
		virtual void CullMode(xna::CullMode value) = 0;
		virtual xna::FillMode FillMode() const = 0;
		virtual void FillMode(xna::FillMode value) = 0;
	};
}

#endif