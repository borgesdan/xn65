#include "xna/xna-dx.hpp"

namespace xna {	
	static void setDefaultTexture2DDesc(Texture2D::PlatformImplementation& impl);
	static HRESULT internalTexture2DSetData(Texture2D::PlatformImplementation& impl, GraphicsDevice& device, UINT const* data);

	Texture2D::Texture2D() : Texture(nullptr) {
		impl = unew<PlatformImplementation>();
		setDefaultTexture2DDesc(*impl);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height) : Texture(device), width(width), height(height) {
		impl = unew<PlatformImplementation>();
		setDefaultTexture2DDesc(*impl);
		impl->dxDescription.Width = static_cast<UINT>(this->width);
		impl->dxDescription.Height = static_cast<UINT>(this->height);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device) : Texture(device) {
		impl = unew<PlatformImplementation>();
		setDefaultTexture2DDesc(*impl);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format)
		: Texture(device), width(width), height(height), levelCount(mipMap) {
		impl = unew<PlatformImplementation>();
		setDefaultTexture2DDesc(*impl);
		impl->dxDescription.Width = static_cast<UINT>(this->width);
		impl->dxDescription.Height = static_cast<UINT>(this->height);
		impl->dxDescription.MipLevels = static_cast<UINT>(this->levelCount);
		impl->dxDescription.Format = DxHelpers::SurfaceFormatToDx(format);
	}

	void Texture2D::Initialize()
	{
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		HRESULT hr = 0;

		if (!impl->dxTexture2D) {
			hr = m_device->impl->_device->CreateTexture2D(&impl->dxDescription, nullptr, impl->dxTexture2D.ReleaseAndGetAddressOf());

			if (FAILED(hr)) {
				Exception::Throw(Exception::FAILED_TO_CREATE);
			}
		}
		else {
			impl->dxTexture2D->GetDesc(&impl->dxDescription);
		}

		comptr<ID3D11Resource> resource = nullptr;
		hr = impl->dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}
		
		if (impl->dxDescription.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
			hr = m_device->impl->_device->CreateShaderResourceView(resource.Get(), &impl->dxShaderDescription, impl->dxShaderResource.ReleaseAndGetAddressOf());

			if (FAILED(hr)) {
				Exception::Throw(Exception::FAILED_TO_CREATE);
			}
		}		

		surfaceFormat = DxHelpers::SurfaceFormatToXna(impl->dxDescription.Format);
		levelCount = static_cast<Int>(impl->dxShaderDescription.Texture2D.MipLevels);
		width = static_cast<Int>(impl->dxDescription.Width);
		height = static_cast<Int>(impl->dxDescription.Height);
	}		

	void Texture2D::SetData(std::vector<Uint> const& data, size_t startIndex, size_t elementCount)
	{
		if (!impl || !m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}		

		internalTexture2DSetData(*impl, *m_device, data.data());
	}

	void Texture2D::SetData(std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(Exception::INVALID_OPERATION);
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

		internalTexture2DSetData(*impl, *m_device, finalData.data());
	}	

	void Texture2D::SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(Exception::INVALID_OPERATION);
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
			auto hr = m_device->impl->_device->CreateTexture2D(&impl->dxDescription, nullptr, impl->dxTexture2D.GetAddressOf());

			if (FAILED(hr)) {
				Exception::Throw(Exception::FAILED_TO_CREATE);
			}
		}

		comptr<ID3D11Resource> resource = nullptr;
		auto hr = impl->dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		D3D11_BOX box{};

		if (rect) {
			box.left = rect->Left();
			box.right = rect->Right();
			box.top = rect->Top();
			box.bottom = rect->Bottom();
			box.back = level;
			box.front = 0;
		}

		constexpr int R8G8B8A8U_BYTE_SIZE = 4;
		m_device->impl->_context->UpdateSubresource(resource.Get(), 0, rect ? &box : nullptr, finalData.data(), impl->dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);		

		impl->dxShaderDescription.Format = impl->dxDescription.Format;
		impl->dxShaderDescription.Texture2D.MipLevels = impl->dxDescription.MipLevels;
		hr = m_device->impl->_device->CreateShaderResourceView(resource.Get(), &impl->dxShaderDescription, impl->dxShaderResource.ReleaseAndGetAddressOf());		

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		impl->dxTexture2D->GetDesc(&impl->dxDescription);
	}

	void Texture2D::SetData(std::vector<Color> const& data, size_t startIndex, size_t elementCount)
	{
		if (!m_device || !m_device->impl->_device || !m_device->impl->_context) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		std::vector<UINT> finalData(elementCount);
		auto finalDataIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			finalData[finalDataIndex] = static_cast<UINT>(data[i]);
			++finalDataIndex;
		}	

		internalTexture2DSetData(*impl, *m_device, finalData.data());
	}

	P_Texture2D Texture2D::FromStream(GraphicsDevice& device, P_Stream const& stream)
	{
		std::vector<Byte> data;
		const auto lenght = stream->Length();
		stream->Read(data, 0, lenght - 1);

		return FromStream(device, data);
	}

	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, String const& fileName)
	{
		auto _this = device.shared_from_this();
		auto texture2d = snew<Texture2D>(_this);
		comptr<ID3D11Resource> resource = nullptr;
		auto wstr = XnaHelper::ToWString(fileName);

		HRESULT result = DirectX::CreateWICTextureFromFile(
			device.impl->_device.Get(),
			device.impl->_context.Get(),
			wstr.c_str(),
			resource.GetAddressOf(),
			texture2d->impl->dxShaderResource.ReleaseAndGetAddressOf(),
			0U);

		if (FAILED(result)) {
			return nullptr;
		}

		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)texture2d->impl->dxTexture2D.ReleaseAndGetAddressOf());

		if (FAILED(result)) {
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->impl->dxTexture2D->GetDesc(&desc);
		texture2d->impl->dxDescription = desc;

		return texture2d;
	}

	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, std::vector<Byte> const& data)
	{
		auto _this = device.shared_from_this();
		auto texture2d = snew<Texture2D>(_this);
		comptr<ID3D11Resource> resource = nullptr;

		auto hr = DirectX::CreateWICTextureFromMemory(
			device.impl->_device.Get(),
			device.impl->_context.Get(),
			data.data(),
			data.size(),
			resource.GetAddressOf(),
			texture2d->impl->dxShaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr))
		{
			return nullptr;
		}
		
		hr = resource->QueryInterface(IID_ID3D11Texture2D, (void**)texture2d->impl->dxTexture2D.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->impl->dxTexture2D->GetDesc(&desc);
		texture2d->impl->dxDescription = desc;

		return texture2d;
	}		

	HRESULT internalTexture2DSetData(Texture2D::PlatformImplementation& impl, GraphicsDevice& device, UINT const* data)
	{
		if (!impl.dxTexture2D) {
			auto hr = device.impl->_device->CreateTexture2D(&impl.dxDescription, nullptr, impl.dxTexture2D.ReleaseAndGetAddressOf());

			if (FAILED(hr)) {
				Exception::Throw(Exception::FAILED_TO_CREATE);
			}
		}

		comptr<ID3D11Resource> resource = nullptr;
		auto hr = impl.dxTexture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		constexpr int R8G8B8A8U_BYTE_SIZE = 4;
		device.impl->_context->UpdateSubresource(resource.Get(), 0, nullptr, data, impl.dxDescription.Width * R8G8B8A8U_BYTE_SIZE, 0);

		impl.dxShaderDescription.Texture2D.MipLevels = impl.dxDescription.MipLevels;
		hr = device.impl->_device->CreateShaderResourceView(resource.Get(), &impl.dxShaderDescription, impl.dxShaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		impl.dxTexture2D->GetDesc(&impl.dxDescription);

		return NO_ERROR;
	}

	void setDefaultTexture2DDesc(Texture2D::PlatformImplementation& impl) {
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
}