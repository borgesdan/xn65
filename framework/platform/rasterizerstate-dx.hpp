#ifndef XNA_PLATFORM_RASTERIZERSTATE_DX_HPP
#define XNA_PLATFORM_RASTERIZERSTATE_DX_HPP

#include "../graphics/rasterizerstate.hpp"
#include "dxheaders.hpp"

namespace xna {
	class RasterizerState : public IRasterizerState {
	public:
		RasterizerState(){}

		virtual ~RasterizerState() override {
			if (_rasterizerState) {
				_rasterizerState->Release();
				_rasterizerState = nullptr;
			}
		}

		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) override;

		virtual constexpr xna::CullMode CullMode() const override {
			switch (_description.CullMode)
			{
			case D3D11_CULL_NONE:
				return xna::CullMode::None;				
			case D3D11_CULL_FRONT:
				return xna::CullMode::CullClockwiseFace;
			case D3D11_CULL_BACK:
				return xna::CullMode::CullCounterClockwiseFace;				
			default:
				return xna::CullMode::None;				
			}
		}

		virtual constexpr void CullMode(xna::CullMode value) override {
			switch (value)
			{
			case xna::CullMode::None:
				_description.CullMode = D3D11_CULL_NONE;
				break;
			case xna::CullMode::CullClockwiseFace:
				_description.CullMode = D3D11_CULL_FRONT;
				break;
			case xna::CullMode::CullCounterClockwiseFace:
				_description.CullMode = D3D11_CULL_BACK;
				break;
			default:
				_description.CullMode = D3D11_CULL_NONE;
				break;
			}
		}

		virtual constexpr xna::FillMode FillMode() const override {
			switch (_description.FillMode) {
			case D3D11_FILL_SOLID:
				return xna::FillMode::Solid;
			case D3D11_FILL_WIREFRAME:
				return xna::FillMode::WireFrame;
			default:
				return xna::FillMode::Solid;
			}
		}

		virtual constexpr void FillMode(xna::FillMode value) override {
			switch (value)
			{
			case xna::FillMode::Solid:
				_description.FillMode = D3D11_FILL_SOLID;
				break;
			case xna::FillMode::WireFrame:
				_description.FillMode = D3D11_FILL_WIREFRAME;
				break;
			default:
				_description.FillMode = D3D11_FILL_SOLID;
				break;
			}
		}

		static PRasterizerState CullNone() {
			auto raster = New<RasterizerState>();
			raster->_description.FillMode = D3D11_FILL_SOLID;
			raster->_description.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
			raster->_description.DepthClipEnable = true;
			return raster;
		}

		static PRasterizerState CullClockwise() {
			auto raster = New<RasterizerState>();
			raster->_description.FillMode = D3D11_FILL_SOLID;
			raster->_description.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
			raster->_description.DepthClipEnable = true;
			return raster;
		}

		static PRasterizerState CullCounterClockwise() {
			auto raster = New<RasterizerState>();
			raster->_description.FillMode = D3D11_FILL_SOLID;
			raster->_description.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
			raster->_description.DepthClipEnable = true;
			return raster;
		}

	public:
		ID3D11RasterizerState* _rasterizerState = nullptr;
		D3D11_RASTERIZER_DESC _description{};
	};
}

#endif