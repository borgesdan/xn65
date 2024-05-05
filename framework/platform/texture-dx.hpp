#ifndef XNA_PLATFORM_TEXTURE_DX_HPP
#define XNA_PLATFORM_TEXTURE_DX_HPP

#include "../common/rectangle.hpp"
#include "../csharp/buffer.hpp"
#include "../graphics/texture.hpp"
#include "../xnaerror.hpp"
#include "dxheaders.hpp"
#include <WICTextureLoader.h>
#include "../graphics/gresource.hpp"
#include "device-dx.hpp"
#include <SimpleMath.h>

namespace xna {
	class Texture2D : public ITexture2D, public GraphicsResource {
	public:
		Texture2D(GraphicsDevice* device, size_t width, size_t height) : GraphicsResource(device) {
			dxDescription.Width = static_cast<UINT>(width);
			dxDescription.Height = static_cast<UINT>(height);
			setDefaultDesc();
		}

		Texture2D(GraphicsDevice* device) : GraphicsResource(device) {
			setDefaultDesc();
		}

		virtual ~Texture2D() override {
			if (dxTexture2D) {
				dxTexture2D->Release();
				dxTexture2D = nullptr;
			}

			if (dxShaderResourceView) {
				dxShaderResourceView->Release();
				dxShaderResourceView = nullptr;
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

		template <typename T>
		void SetData(std::vector<T> const& data, xna_error_ptr_arg);

		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount, xna_error_nullarg);

		static sptr<Texture2D> FromStream(GraphicsDevice& device, String const& fileName, xna_error_nullarg);

	public:
		ID3D11Texture2D* dxTexture2D{ nullptr };
		ID3D11ShaderResourceView* dxShaderResourceView{ nullptr };
		D3D11_SUBRESOURCE_DATA dxSubResource{};
		D3D11_TEXTURE2D_DESC dxDescription{};
		D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderDecription{};

	private:
		static constexpr int R8G8B8A8U_BYTE_SIZE = 4;

		void setDefaultDesc() {
			dxDescription.MipLevels = 1;
			dxDescription.ArraySize = 1;
			dxDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxDescription.SampleDesc.Count = 1;
			dxDescription.Usage = D3D11_USAGE_DEFAULT;
			dxDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			dxShaderDecription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			dxShaderDecription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			dxShaderDecription.Texture2D.MipLevels = dxDescription.MipLevels;
			dxShaderDecription.Texture2D.MostDetailedMip = 0;
		}
	};

	template <typename T>
	inline void Texture2D::SetData(std::vector<T> const& data, xna_error_ptr_arg) {
		SetData(data, 0, data.size(), err);
	}

	template <typename T>
	inline void Texture2D::SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount, xna_error_ptr_arg) {
		if (!m_device || !m_device->_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		std::vector<UINT> finalData(elementCount);
		auto finalDataIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			finalData[finalDataIndex] = static_cast<UINT>(data[i]);
			++finalDataIndex;
		}
		
		if (!dxTexture2D) {
			auto hr = m_device->_device->CreateTexture2D(&dxDescription, nullptr, &dxTexture2D);

			if (FAILED(hr)) {
				xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
				return;
			}
		}		

		ID3D11Resource* resource = nullptr;
		auto hr = dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return;
		}

		dxShaderDecription.Texture2D.MipLevels = dxDescription.MipLevels;
		m_device->_context->UpdateSubresource(resource, 0, nullptr, finalData.data(), dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);	
		
		if (dxShaderResourceView) {
			dxShaderResourceView->Release();
			dxShaderResourceView = nullptr;
		}

		hr = m_device->_device->CreateShaderResourceView(resource, &dxShaderDecription, &dxShaderResourceView);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return;
		}

		dxTexture2D->GetDesc(&dxDescription);
	}
}

#endif