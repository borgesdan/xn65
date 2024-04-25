#ifndef XNA_PLATFORM_DEPTHSTENCILSTATE_DX_HPP
#define XNA_PLATFORM_DEPTHSTENCILSTATE_DX_HPP

#include "../graphics/depthstencilstate.hpp"
#include "../graphics/gresource.hpp"
#include "dxheaders.hpp"

namespace xna {
	class DepthStencilState : public IDepthStencilState, public GraphicsResource {
	public:
		DepthStencilState(GraphicsDevice* device) : GraphicsResource(device) {
			dxDescription = defaultDesc();
		}

		virtual ~DepthStencilState() override {
			if (dxDepthStencil) {
				dxDepthStencil->Release();
				dxDepthStencil = nullptr;
			}
		}

		virtual bool Initialize(xna_error_nullarg) override;
		virtual bool Apply(xna_error_ptr_arg) override;

		virtual constexpr void DepthEnabled(bool value) override {
			dxDescription.DepthEnable = value;
		}

		virtual constexpr void DepthWriteEnabled(bool value) override {
			dxDescription.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(value);
		}

		virtual constexpr void DepthCompareFunction(ComparisonFunction value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
		}

		virtual constexpr void StencilEnabled(bool value) override {
			dxDescription.StencilEnable = value;
		}

		virtual constexpr void StencilReadMask(int value) override {
			dxDescription.StencilReadMask = static_cast<UINT8>(value);
		}

		virtual constexpr void StencilWriteMask(int value) override {
			dxDescription.StencilWriteMask = static_cast<UINT8>(value);
		}

		virtual constexpr void StencilFrontFacePass(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilFrontFaceFail(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilFrontFaceDepthFail(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilFrontFaceCompare(ComparisonFunction value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
		}

		virtual constexpr void StencilBackFacePass(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilBackFaceFail(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilBackFaceDepthFail(StencilOperation value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
		}

		virtual constexpr void StencilBackFaceCompare(ComparisonFunction value) override {
			const auto _value = static_cast<int>(value) + 1;
			dxDescription.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
		}

		virtual constexpr bool DepthEnabled() override {
			return dxDescription.DepthEnable;
		}

		virtual constexpr bool DepthWriteEnabled() override {
			return static_cast<bool>(dxDescription.DepthWriteMask);
		}

		virtual constexpr ComparisonFunction DepthCompareFunction() override {
			const auto _value = static_cast<int>(dxDescription.DepthFunc) - 1;
			return static_cast<ComparisonFunction>(_value);
		}

		virtual constexpr bool StencilEnabled() override {
			return dxDescription.StencilEnable;
		}

		virtual constexpr int StencilReadMask() override {
			return static_cast<int>(dxDescription.StencilReadMask);
		}

		virtual constexpr int StencilWriteMask() override {
			return static_cast<int>(dxDescription.StencilWriteMask);
		}

		virtual constexpr StencilOperation StencilFrontFacePass() override {
			const auto _value = static_cast<int>(dxDescription.FrontFace.StencilPassOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr StencilOperation StencilFrontFaceFail() override {
			const auto _value = static_cast<int>(dxDescription.FrontFace.StencilFailOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr StencilOperation StencilFrontFaceDepthFail() override {
			const auto _value = static_cast<int>(dxDescription.FrontFace.StencilDepthFailOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr ComparisonFunction StencilFrontFaceCompare() override {
			const auto _value = static_cast<int>(dxDescription.FrontFace.StencilFunc) - 1;
			return static_cast<ComparisonFunction>(_value);
		}

		virtual constexpr StencilOperation StencilBackFacePass() override {
			const auto _value = static_cast<int>(dxDescription.BackFace.StencilPassOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr StencilOperation StencilBackFaceFail() override {
			const auto _value = static_cast<int>(dxDescription.BackFace.StencilFailOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr StencilOperation StencilBackFaceDepthFail() override {
			const auto _value = static_cast<int>(dxDescription.BackFace.StencilDepthFailOp) - 1;
			return static_cast<StencilOperation>(_value);
		}

		virtual constexpr ComparisonFunction StencilBackFaceCompare() override {
			const auto _value = static_cast<int>(dxDescription.BackFace.StencilFunc) - 1;
			return static_cast<ComparisonFunction>(_value);
		}

		static uptr<DepthStencilState> None();
		static uptr<DepthStencilState> Default();
		static uptr<DepthStencilState> DepthRead();

	public:
		ID3D11DepthStencilState* dxDepthStencil = nullptr;
		D3D11_DEPTH_STENCIL_DESC dxDescription{};

	private:
		DepthStencilState() : GraphicsResource(nullptr) {
			dxDescription = defaultDesc();
		}

		static D3D11_DEPTH_STENCIL_DESC defaultDesc() {
			D3D11_DEPTH_STENCIL_DESC _description{};
			_description.DepthEnable = true;
			_description.StencilEnable = true;
			_description.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			_description.StencilReadMask = 0;
			_description.StencilWriteMask = 0;
			_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

			return _description;
		}
	};
}

#endif