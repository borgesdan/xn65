#ifndef XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP
#define XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP

#include "../default.hpp"

namespace xna {
	class IDepthStencilState {
	public:
		virtual ~IDepthStencilState(){}
		virtual bool Initialize(xna_error_nullarg) = 0;
		virtual bool Apply(xna_error_ptr_arg) = 0;
		
		virtual void DepthEnabled(bool value) = 0;
		virtual void DepthWriteEnabled(bool value) = 0;
		virtual void DepthCompareFunction(ComparisonFunction value) = 0;
		virtual void StencilEnabled(bool value) = 0;
		virtual void StencilReadMask(int value) = 0;
		virtual void StencilWriteMask(int value) = 0;
		virtual void StencilFrontFacePass(StencilOperation value) = 0;
		virtual void StencilFrontFaceFail(StencilOperation value) = 0;
		virtual void StencilFrontFaceDepthFail(StencilOperation value) = 0;
		virtual void StencilFrontFaceCompare(ComparisonFunction value) = 0;
		virtual void StencilBackFacePass(StencilOperation value) = 0;
		virtual void StencilBackFaceFail(StencilOperation value) = 0;
		virtual void StencilBackFaceDepthFail(StencilOperation value) = 0;
		virtual void StencilBackFaceCompare(ComparisonFunction value) = 0;

		virtual bool DepthEnabled() = 0;
		virtual bool DepthWriteEnabled() = 0;
		virtual ComparisonFunction DepthCompareFunction() = 0;
		virtual bool StencilEnabled() = 0;
		virtual int StencilReadMask() = 0;
		virtual int StencilWriteMask() = 0;
		virtual StencilOperation StencilFrontFacePass() = 0;
		virtual StencilOperation StencilFrontFaceFail() = 0;
		virtual StencilOperation StencilFrontFaceDepthFail() = 0;
		virtual ComparisonFunction StencilFrontFaceCompare() = 0;
		virtual StencilOperation StencilBackFacePass() = 0;
		virtual StencilOperation StencilBackFaceFail() = 0;
		virtual StencilOperation StencilBackFaceDepthFail() = 0;
		virtual ComparisonFunction StencilBackFaceCompare() = 0;		
	};
}

#endif