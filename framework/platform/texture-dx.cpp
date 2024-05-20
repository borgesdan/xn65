#include "platform-dx/texture-dx.hpp"
#include "platform-dx/device-dx.hpp"
#include "graphics/adapter.hpp"
#include "platform-dx/implementations.hpp"
#include "platform-dx/dxhelpers.hpp"

namespace xna {	
	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, String const& fileName, xna_error_ptr_arg)
	{		
		auto _this = device.shared_from_this();
		auto texture2d = New<Texture2D>(_this);
		ID3D11Resource* resource = nullptr;		
		auto wstr = XnaHToWString(fileName);
		
		HRESULT result = DirectX::CreateWICTextureFromFile(
			device._device,
			device._context,
			wstr.c_str(),
			&resource,
			&texture2d->dxShaderResource,
			0U);		

		if (FAILED(result))
		{
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}
		
		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->dxTexture2D);

		if (FAILED(result)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}			

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->dxTexture2D->GetDesc(&desc);
		texture2d->dxDescription = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}

	bool Texture2D::Initialize(xna_error_ptr_arg)
	{
		if (dxTexture2D) {
			xna_error_apply(err, XnaErrorCode::WARNING_INITIALIZED_RESOURCE);
			return false;
		}

		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		auto hr = m_device->_device->CreateTexture2D(&dxDescription, nullptr, &dxTexture2D);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		ID3D11Resource* resource = nullptr;
		hr = dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		hr = m_device->_device->CreateShaderResourceView(resource, &dxShaderDescription, &dxShaderResource);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height) : GraphicsResource(device) {
		setDefaultDesc();
		dxDescription.Width = static_cast<UINT>(width);
		dxDescription.Height = static_cast<UINT>(height);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		setDefaultDesc();
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format) : GraphicsResource(device)
	{
		setDefaultDesc();
		dxDescription.Width = static_cast<UINT>(width);
		dxDescription.Height = static_cast<UINT>(height);
		dxDescription.MipLevels = static_cast<UINT>(mipMap);
		dxDescription.Format = DxHelpers::ConvertSurfaceToDXGIFORMAT(format);
	}

	void Texture2D::SetData(std::vector<Uint> const& data, size_t startIndex, size_t elementCount, xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}		

		internalSetData(data.data(), err);
	}

	void Texture2D::SetData(std::vector<Byte> const& data, size_t startIndex, size_t elementCount, xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		std::vector<UINT> finalData(elementCount / 4);
		auto fIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			const auto& r = data[i];
			const auto& g = data[++i];
			const auto& b = data[++i];
			const auto& a = data[++i];
			finalData[fIndex] = Color(r, g, b, a);
			++fIndex;
		}

		internalSetData(finalData.data(), err);
	}

	void Texture2D::SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount, xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return;
		}

		std::vector<UINT> finalData(elementCount / 4);
		auto fIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			const auto& r = data[i];
			const auto& g = data[++i];
			const auto& b = data[++i];
			const auto& a = data[++i];
			finalData[fIndex] = Color(r, g, b, a);
			++fIndex;
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

		D3D11_BOX box{};

		if (rect) {
			box.left = rect->X;
			box.right = rect->X + rect->Width;
			box.top = rect->Y;
			box.bottom = rect->Y + rect->Height;
			box.back = level;
			box.front = 0;
		}

		m_device->_context->UpdateSubresource(resource, 0, rect ? &box : nullptr, finalData.data(), dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);

		if (dxShaderResource) {
			dxShaderResource->Release();
			dxShaderResource = nullptr;
		}

		dxShaderDescription.Texture2D.MipLevels = dxDescription.MipLevels;
		hr = m_device->_device->CreateShaderResourceView(resource, &dxShaderDescription, &dxShaderResource);

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

	void Texture2D::SetData(std::vector<Color> const& data, size_t startIndex, size_t elementCount, xna_error_ptr_arg)
	{
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

		internalSetData(finalData.data(), err);
	}

	sptr<Texture2D> Texture2D::FromMemory(GraphicsDevice& device, std::vector<Byte> const& data, xna_error_ptr_arg)
	{
		auto _this = device.shared_from_this();
		auto texture2d = New<Texture2D>(_this);
		ID3D11Resource* resource = nullptr;

		auto hr = DirectX::CreateWICTextureFromMemory(
			device._device,
			device._context,
			data.data(),
			data.size(),
			&resource,
			&texture2d->dxShaderResource);

		if (FAILED(hr))
		{
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}
		
		hr = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->dxTexture2D);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);

			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->dxTexture2D->GetDesc(&desc);
		texture2d->dxDescription = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}

	void Texture2D::internalSetData(UINT const* data, xna_error_ptr_arg)
	{
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

		m_device->_context->UpdateSubresource(resource, 0, nullptr, data, dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);

		if (dxShaderResource) {
			dxShaderResource->Release();
			dxShaderResource = nullptr;
		}

		dxShaderDescription.Texture2D.MipLevels = dxDescription.MipLevels;
		hr = m_device->_device->CreateShaderResourceView(resource, &dxShaderDescription, &dxShaderResource);

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