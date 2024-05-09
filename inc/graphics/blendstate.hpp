#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../default.hpp"

namespace xna {
	struct BlendRenderTarget;

	class IBlendState {
	public:
		virtual ~IBlendState() {}
		virtual bool Initialize(xna_error_nullarg) = 0;
		virtual void AlphaToCoverageEnable(bool value) = 0;
		virtual void IndependentBlendEnable(bool value) = 0;
		virtual void RenderTargets(std::vector<BlendRenderTarget> const& value) = 0;

		virtual bool Apply(xna_error_nullarg) = 0;		
	};	
}

#endif