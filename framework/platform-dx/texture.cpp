#include "xna/platform-dx/dx.hpp"

namespace xna {	
	Texture2D::~Texture2D() {
		impl = nullptr;
	}

	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, String const& fileName)
	{		
		auto _this = device.shared_from_this();
		auto texture2d = New<Texture2D>(_this);
		ID3D11Resource* resource = nullptr;		
		auto wstr = XnaHelper::ToWString(fileName);
		
		HRESULT result = DirectX::CreateWICTextureFromFile(
			device.impl->_device,
			device.impl->_context,
			wstr.c_str(),
			&resource,
			&texture2d->impl->dxShaderResource,
			0U);		

		if (FAILED(result))
		{			
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}
		
		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->impl->dxTexture2D);

		if (FAILED(result)) {
			if (resource) {
				resource->Release();
				resource = nullptr;
			}			

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->impl->dxTexture2D->GetDesc(&desc);
		texture2d->impl->dxDescription = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}

	bool Texture2D::Initialize()
	{
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
		}

		auto hr = m_device->impl->_device->CreateTexture2D(&impl->dxDescription, nullptr, &impl->dxTexture2D);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		ID3D11Resource* resource = nullptr;
		hr = impl->dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		hr = m_device->impl->_device->CreateShaderResourceView(resource, &impl->dxShaderDescription, &impl->dxShaderResource);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	void setDefaultDesc(Texture2D::PlatformImplementation& impl) {
		impl.dxDescription.MipLevels = 1;
		impl.dxDescription.ArraySize = 1;
		impl.dxDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		impl.dxDescription.SampleDesc.Count = 1;
		impl.dxDescription.Usage = D3D11_USAGE_DEFAULT;
		impl.dxDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	
		impl.dxShaderDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		impl.dxShaderDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		impl.dxShaderDescription.Texture2D.MipLevels = impl.dxDescription.MipLevels;
		impl.dxShaderDescription.Texture2D.MostDetailedMip = 0;
	}

	Texture2D::Texture2D() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
		setDefaultDesc(*impl);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height) : GraphicsResource(device) {		
		impl = unew<PlatformImplementation>();
		setDefaultDesc(*impl);
		impl->dxDescription.Width = static_cast<UINT>(width);
		impl->dxDescription.Height = static_cast<UINT>(height);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
		setDefaultDesc(*impl);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format) : GraphicsResource(device)
	{
		impl = unew<PlatformImplementation>();
		setDefaultDesc(*impl);
		impl->dxDescription.Width = static_cast<UINT>(width);
		impl->dxDescription.Height = static_cast<UINT>(height);
		impl->dxDescription.MipLevels = static_cast<UINT>(mipMap);
		impl->dxDescription.Format = DxHelpers::ConvertSurfaceToDXGIFORMAT(format);
	}

	HRESULT internalSetData(Texture2D::PlatformImplementation& impl, GraphicsDevice& device,  UINT const* data)
	{
		if (!impl.dxTexture2D) {
			auto hr = device.impl->_device->CreateTexture2D(&impl.dxDescription, nullptr, &impl.dxTexture2D);

			if (FAILED(hr)) {
				Exception::Throw(ExMessage::CreateComponent);
			}
		}

		ID3D11Resource* resource = nullptr;
		auto hr = impl.dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		constexpr int R8G8B8A8U_BYTE_SIZE = 4;
		device.impl->_context->UpdateSubresource(resource, 0, nullptr, data, impl.dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);

		if (impl.dxShaderResource) {
			impl.dxShaderResource->Release();
			impl.dxShaderResource = nullptr;
		}

		impl.dxShaderDescription.Texture2D.MipLevels = impl.dxDescription.MipLevels;
		hr = device.impl->_device->CreateShaderResourceView(resource, &impl.dxShaderDescription, &impl.dxShaderResource);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		impl.dxTexture2D->GetDesc(&impl.dxDescription);

		return NO_ERROR;
	}

	void Texture2D::SetData(std::vector<Uint> const& data, size_t startIndex, size_t elementCount)
	{
		if (!impl || !m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
		}		

		internalSetData(*impl, *m_device, data.data());
	}

	void Texture2D::SetData(std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
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

		internalSetData(*impl, *m_device, finalData.data());
	}

	void Texture2D::SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
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

		if (!impl->dxTexture2D) {
			auto hr = m_device->impl->_device->CreateTexture2D(&impl->dxDescription, nullptr, &impl->dxTexture2D);

			if (FAILED(hr)) {
				Exception::Throw(ExMessage::CreateComponent);
			}
		}

		ID3D11Resource* resource = nullptr;
		auto hr = impl->dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)&resource);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::InvalidOperation);
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

		constexpr int R8G8B8A8U_BYTE_SIZE = 4;
		m_device->impl->_context->UpdateSubresource(resource, 0, rect ? &box : nullptr, finalData.data(), impl->dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);

		if (impl->dxShaderResource) {
			impl->dxShaderResource->Release();
			impl->dxShaderResource = nullptr;
		}

		impl->dxShaderDescription.Format = impl->dxDescription.Format;
		impl->dxShaderDescription.Texture2D.MipLevels = impl->dxDescription.MipLevels;
		hr = m_device->impl->_device->CreateShaderResourceView(resource, &impl->dxShaderDescription, &impl->dxShaderResource);

		if (resource) {
			resource->Release();
			resource = nullptr;
		}

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		impl->dxTexture2D->GetDesc(&impl->dxDescription);
	}

	void Texture2D::SetData(std::vector<Color> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		std::vector<UINT> finalData(elementCount);
		auto finalDataIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			finalData[finalDataIndex] = static_cast<UINT>(data[i]);
			++finalDataIndex;
		}	

		internalSetData(*impl, *m_device, finalData.data());
	}

	sptr<Texture2D> Texture2D::FromMemory(GraphicsDevice& device, std::vector<Byte> const& data)
	{
		auto _this = device.shared_from_this();
		auto texture2d = New<Texture2D>(_this);
		ID3D11Resource* resource = nullptr;

		auto hr = DirectX::CreateWICTextureFromMemory(
			device.impl->_device,
			device.impl->_context,
			data.data(),
			data.size(),
			&resource,
			&texture2d->impl->dxShaderResource);

		if (FAILED(hr))
		{
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}
		
		hr = resource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2d->impl->dxTexture2D);

		if (FAILED(hr)) {
			if (resource) {
				resource->Release();
				resource = nullptr;
			}

			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->impl->dxTexture2D->GetDesc(&desc);
		texture2d->impl->dxDescription = desc;

		resource->Release();
		resource = nullptr;

		return texture2d;
	}	

	Int Texture2D::Width() const {
		if (!impl) return 0;

		return static_cast<Int>(impl->dxDescription.Width);
	}

	Int Texture2D::Height() const {
		if (!impl) return 0;

		return static_cast<Int>(impl->dxDescription.Height);
	}

	Rectangle Texture2D::Bounds() const {
		if (!impl) return {};

		return Rectangle(
			0, 0,
			static_cast<Int>(impl->dxDescription.Width),
			static_cast<Int>(impl->dxDescription.Height)
		);
	}
}