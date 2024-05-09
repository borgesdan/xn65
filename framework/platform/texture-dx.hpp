#ifndef XNA_PLATFORM_TEXTURE_DX_HPP
#define XNA_PLATFORM_TEXTURE_DX_HPP

#include "../common/rectangle.hpp"
#include "../graphics/gresource.hpp"
#include "../graphics/texture.hpp"
#include "dxheaders.hpp"
#include "device-dx.hpp"

namespace xna {
	class Texture2D : public ITexture2D, public GraphicsResource {
	public:
		Texture2D() : GraphicsResource(nullptr){
			setDefaultDesc();
		}

		Texture2D(sptr<GraphicsDevice> const& device);
		Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height);
		Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format);

		virtual ~Texture2D() override {
			if (dxTexture2D) {
				dxTexture2D->Release();
				dxTexture2D = nullptr;
			}

			if (dxShaderResource) {
				dxShaderResource->Release();
				dxShaderResource = nullptr;
			}
		}

		virtual constexpr Int Width() const override {
			return dxDescription.Width;
		}

		virtual constexpr Int Height() const override {
			return dxDescription.Height;
		}

		constexpr Rectangle Bounds() const override {
			return { 0, 0, static_cast<Int>(dxDescription.Width), static_cast<Int>(dxDescription.Height) };
		}

		bool Initialize(xna_error_nullarg) override;		
		
		void SetData(std::vector<Color> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		void SetData(std::vector<Uint> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		void SetData(std::vector<Byte> const& data, size_t startIndex = 0, size_t elementCount = 0, xna_error_nullarg);
		
		void SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount, xna_error_nullarg);

		static sptr<Texture2D> FromStream(GraphicsDevice& device, String const& fileName, xna_error_nullarg);
		static sptr<Texture2D> FromMemory(GraphicsDevice& device, std::vector<Byte> const& data, xna_error_nullarg);

	public:
		ID3D11Texture2D* dxTexture2D{ nullptr };
		ID3D11ShaderResourceView* dxShaderResource{ nullptr };
		D3D11_SUBRESOURCE_DATA dxSubResource{};
		D3D11_TEXTURE2D_DESC dxDescription{};
		D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderDescription{};

	private:
		static constexpr int R8G8B8A8U_BYTE_SIZE = 4;

		void setDefaultDesc() {
			dxDescription.MipLevels = 1;
			dxDescription.ArraySize = 1;
			dxDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxDescription.SampleDesc.Count = 1;
			dxDescription.Usage = D3D11_USAGE_DEFAULT;
			dxDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			dxShaderDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxShaderDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			dxShaderDescription.Texture2D.MipLevels = dxDescription.MipLevels;
			dxShaderDescription.Texture2D.MostDetailedMip = 0;
		}

		void internalSetData(UINT const* data, xna_error_nullarg);
	};		
}

#endif