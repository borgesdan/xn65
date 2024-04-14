#ifndef XNA_PLATFORM_TEXTURE_DX_HPP
#define XNA_PLATFORM_TEXTURE_DX_HPP

#include "../graphics/texture.hpp"
#include "dxgi.h"
#include "d3d11.h"
#include "../xnaerror.hpp"

namespace xna {
	class Texture2D : public ITexture2D {
	public:
		Texture2D();

		virtual ~Texture2D() override {
			if (_texture2D) {
				_texture2D->Release();
				_texture2D = nullptr;
			}

			if (_textureView) {
				_textureView->Release();
				_textureView = nullptr;
			}
		}

		virtual constexpr Int Width() const override {
			return _description.Width;
		}

		virtual constexpr Int Height() const override {
			return _description.Height;
		}

		static PTexture2D FromStream(GraphicsDevice& device, String const& fileName, xna_error_nullarg);		

	public:
		ID3D11Texture2D* _texture2D{nullptr};
		ID3D11ShaderResourceView* _textureView{ nullptr };
		D3D11_TEXTURE2D_DESC _description{};
	
	};
}

#endif