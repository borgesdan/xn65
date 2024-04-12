#ifndef XNA_PLATFORM_VERTEXINPUT_DX_HPP
#define XNA_PLATFORM_VERTEXINPUT_DX_HPP

#include "../graphics/vertexinput.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class VertexInputLayout : public IVertexInputLayout {
	public:
		VertexInputLayout() = default;

		VertexInputLayout(
			std::vector<D3D11_INPUT_ELEMENT_DESC> const& description) :
			_description(description){}

		virtual ~VertexInputLayout() override{
			if (_inputLayout) {
				_inputLayout->Release();
				_inputLayout = nullptr;
			}
		}		

		virtual bool Initialize(GraphicsDevice& device, DataBuffer& blob, xna_error_nullarg);

	public:
		ID3D11InputLayout* _inputLayout{ nullptr };
		std::vector<D3D11_INPUT_ELEMENT_DESC> _description{};
	};
}

#endif