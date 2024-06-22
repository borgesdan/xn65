#ifndef XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP
#define XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class DepthStencilState : public GraphicsResource {
	public:

		DepthStencilState();
		DepthStencilState(sptr<GraphicsDevice> const& device);

		~DepthStencilState() override;
		bool Initialize();
		bool Apply();
		
		void DepthEnabled(bool value);
		void DepthWriteEnabled(bool value);
		void DepthCompareFunction(ComparisonFunction value);
		void StencilEnabled(bool value);
		void StencilReadMask(Int value);
		void StencilWriteMask(Int value);
		void StencilFrontFacePass(StencilOperation value);
		void StencilFrontFaceFail(StencilOperation value);
		void StencilFrontFaceDepthFail(StencilOperation value);
		void StencilFrontFaceCompare(ComparisonFunction value);
		void StencilBackFacePass(StencilOperation value);
		void StencilBackFaceFail(StencilOperation value);
		void StencilBackFaceDepthFail(StencilOperation value);
		void StencilBackFaceCompare(ComparisonFunction value);

		bool DepthEnabled() const;
		bool DepthWriteEnabled() const;
		ComparisonFunction DepthCompareFunction() const;
		bool StencilEnabled() const;
		Int StencilReadMask() const;
		Int StencilWriteMask() const;
		StencilOperation StencilFrontFacePass() const;
		StencilOperation StencilFrontFaceFail() const;
		StencilOperation StencilFrontFaceDepthFail() const;
		ComparisonFunction StencilFrontFaceCompare() const;
		StencilOperation StencilBackFacePass() const;
		StencilOperation StencilBackFaceFail() const;
		StencilOperation StencilBackFaceDepthFail() const;
		ComparisonFunction StencilBackFaceCompare() const;

		static uptr<DepthStencilState> None();
		static uptr<DepthStencilState> Default();
		static uptr<DepthStencilState> DepthRead();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif