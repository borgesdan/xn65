#ifndef XNA_PLATFORM_RASTERIZERSTATE_DX_HPP
#define XNA_PLATFORM_RASTERIZERSTATE_DX_HPP

#include "../graphics/rasterizerstate.hpp"
#include "../graphics/gresource.hpp"
#include "dxheaders.hpp"

namespace xna {
	class RasterizerState : public IRasterizerState, public GraphicsResource {
	public:
		RasterizerState(GraphicsDevice* device) : GraphicsResource(device){}

		virtual ~RasterizerState() override {
			if (dxRasterizerState) {
				dxRasterizerState->Release();
				dxRasterizerState = nullptr;
			}
		}

		virtual bool Initialize(xna_error_nullarg) override;
		virtual bool Apply(xna_error_nullarg) override;

		virtual constexpr xna::CullMode CullMode() const override {
			return static_cast<xna::CullMode>(static_cast<int>(dxDescription.CullMode) - 1);			
		}

		virtual constexpr void CullMode(xna::CullMode value) override {
			dxDescription.CullMode = static_cast<D3D11_CULL_MODE>(static_cast<int>(value) + 1);
		}

		virtual constexpr xna::FillMode FillMode() const override {
			return static_cast<xna::FillMode>(static_cast<int>(dxDescription.FillMode) - 2);			
		}

		virtual constexpr void FillMode(xna::FillMode value) override {
			dxDescription.FillMode = static_cast<D3D11_FILL_MODE>(static_cast<int>(value) + 2);			
		}

		static uptr<RasterizerState> CullNone();
		static uptr<RasterizerState> CullClockwise();
		static uptr<RasterizerState> CullCounterClockwise();

	public:
		ID3D11RasterizerState* dxRasterizerState = nullptr;
		D3D11_RASTERIZER_DESC dxDescription{};

	private:
		RasterizerState() : GraphicsResource(nullptr){}
	};
}

#endif