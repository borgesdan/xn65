#include "xna-dx/framework.hpp"

namespace xna {	
	HRESULT Texture2DImplementation::SetData(GraphicsDevice& device, UINT const* data) {
		if (!Texture2D) {
			auto hr = device.Implementation->Device->CreateTexture2D(&Description, nullptr, Texture2D.ReleaseAndGetAddressOf());

			if (FAILED(hr)) {
				return hr;
			}
		}

		comptr<ID3D11Resource> resource = nullptr;
		auto hr = Texture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if (FAILED(hr)) {
			return hr;
		}

		constexpr int R8G8B8A8U_BYTE_SIZE = 4;
		device.Implementation->Context->UpdateSubresource(resource.Get(), 0, nullptr, data, Description.Width * R8G8B8A8U_BYTE_SIZE, 0);

		ShaderDescription.Texture2D.MipLevels = Description.MipLevels;
		hr = device.Implementation->Device->CreateShaderResourceView(resource.Get(), &ShaderDescription, ShaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			return hr;
		}

		Texture2D->GetDesc(&Description);

		return NO_ERROR;
	}	

	Texture2D::Texture2D() : Texture(nullptr) {
		Implementation = unew<Texture2DImplementation>();
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height) : Texture(device), width(width), height(height) {
		Implementation = unew<Texture2DImplementation>();
		Implementation->Description.Width = static_cast<UINT>(this->width);
		Implementation->Description.Height = static_cast<UINT>(this->height);
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device) : Texture(device) {
		Implementation = unew<Texture2DImplementation>();
	}

	Texture2D::Texture2D(sptr<GraphicsDevice> const& device, size_t width, size_t height, size_t mipMap, SurfaceFormat format)
		: Texture(device), width(width), height(height), levelCount(mipMap) {
		Implementation = unew<Texture2DImplementation>();
		Implementation->Description.Width = static_cast<UINT>(this->width);
		Implementation->Description.Height = static_cast<UINT>(this->height);
		Implementation->Description.MipLevels = static_cast<UINT>(this->levelCount);
		Implementation->Description.Format = DxHelpers::SurfaceFormatToDx(format);
	}

	void Texture2D::Initialize()
	{
		if (!BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device) {
			throw csharp::InvalidOperationException();
		}		

		auto& deviceImpl = BaseGraphicsDevice->Implementation;

		HRESULT hr = 0;
		
		if (!Implementation->Texture2D) {
			hr = deviceImpl->Device->CreateTexture2D(
				&Implementation->Description, 
				nullptr, 
				Implementation->Texture2D.ReleaseAndGetAddressOf());

			if FAILED(hr) 
				throw csharp::InvalidOperationException();
		}
		else {
			//Updates description if texture is not null
			Implementation->Texture2D->GetDesc(&Implementation->Description);
		}

		comptr<ID3D11Resource> resource = nullptr;
		hr = Implementation->Texture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if FAILED(hr)
			throw csharp::InvalidOperationException();
		
		//Only initializes if it is a ShaderResource
		if (Implementation->Description.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
			hr = deviceImpl->Device->CreateShaderResourceView(
				resource.Get(), 
				&Implementation->ShaderDescription,
				Implementation->ShaderResource.ReleaseAndGetAddressOf());

			if FAILED(hr)
				throw csharp::InvalidOperationException();
			
		}		

		surfaceFormat = DxHelpers::SurfaceFormatToXna(Implementation->Description.Format);
		levelCount = static_cast<Int>(Implementation->ShaderDescription.Texture2D.MipLevels);
		width = static_cast<Int>(Implementation->Description.Width);
		height = static_cast<Int>(Implementation->Description.Height);
	}		

	void Texture2D::SetData(std::vector<Uint> const& data, size_t startIndex, size_t elementCount)
	{
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device || !BaseGraphicsDevice->Implementation->Context) {
			throw csharp::InvalidOperationException();
		}		

		auto hr = Implementation->SetData(*BaseGraphicsDevice, data.data());

		if (FAILED(hr))
			throw csharp::InvalidOperationException();
	}

	void Texture2D::SetData(std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device || !BaseGraphicsDevice->Implementation->Context) {
			throw csharp::InvalidOperationException();
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

		auto hr = Implementation->SetData(*BaseGraphicsDevice, finalData.data());

		if (FAILED(hr))
			throw csharp::InvalidOperationException();
	}	

	void Texture2D::SetData(Int level, Rectangle* rect, std::vector<Byte> const& data, size_t startIndex, size_t elementCount)
	{
		if (!BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device || !BaseGraphicsDevice->Implementation->Context) {
			throw csharp::InvalidOperationException();
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

		if (!Implementation->Texture2D) {
			auto hr = BaseGraphicsDevice->Implementation->Device->CreateTexture2D(&Implementation->Description, nullptr, Implementation->Texture2D.GetAddressOf());

			if (FAILED(hr)) {
				throw csharp::InvalidOperationException();
			}
		}

		comptr<ID3D11Resource> resource = nullptr;
		auto hr = Implementation->Texture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf());

		if (FAILED(hr)) {
			throw csharp::InvalidOperationException();
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
		BaseGraphicsDevice->Implementation->Context->UpdateSubresource(resource.Get(), 0, rect ? &box : nullptr, finalData.data(), Implementation->Description.Width * R8G8B8A8U_BYTE_SIZE, 0);		

		Implementation->ShaderDescription.Format = Implementation->Description.Format;
		Implementation->ShaderDescription.Texture2D.MipLevels = Implementation->Description.MipLevels;
		hr = BaseGraphicsDevice->Implementation->Device->CreateShaderResourceView(resource.Get(), &Implementation->ShaderDescription, Implementation->ShaderResource.ReleaseAndGetAddressOf());		

		if (FAILED(hr)) {
			throw csharp::InvalidOperationException();
		}

		Implementation->Texture2D->GetDesc(&Implementation->Description);
	}

	void Texture2D::SetData(std::vector<Color> const& data, size_t startIndex, size_t elementCount)
	{
		if (!BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device || !BaseGraphicsDevice->Implementation->Context) {
			throw csharp::InvalidOperationException();
		}

		std::vector<UINT> finalData(elementCount);
		auto finalDataIndex = 0;

		for (size_t i = startIndex; i < elementCount; ++i) {
			finalData[finalDataIndex] = static_cast<UINT>(data[i]);
			++finalDataIndex;
		}	

		auto hr = Implementation->SetData(*BaseGraphicsDevice, finalData.data());

		if (FAILED(hr))
			throw csharp::InvalidOperationException();
	}

	P_Texture2D Texture2D::FromStream(GraphicsDevice& device, csharp::Stream& stream)
	{
		std::vector<Byte> data;
		const auto lenght = stream.Length();
		stream.Read(data.data(), data.size(), 0, lenght - 1);

		return FromStream(device, data);
	}

	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, std::string const& fileName)
	{
		auto _this = device.shared_from_this();
		auto texture2d = snew<Texture2D>(_this);
		comptr<ID3D11Resource> resource = nullptr;
		auto wstr = misc::ToWString(fileName);

		HRESULT result = DirectX::CreateWICTextureFromFile(
			device.Implementation->Device.Get(),
			device.Implementation->Context.Get(),
			wstr.c_str(),
			resource.GetAddressOf(),
			texture2d->Implementation->ShaderResource.ReleaseAndGetAddressOf(),
			0U);

		if (FAILED(result)) {
			return nullptr;
		}

		result = resource->QueryInterface(IID_ID3D11Texture2D, (void**)texture2d->Implementation->Texture2D.ReleaseAndGetAddressOf());

		if (FAILED(result)) {
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->Implementation->Texture2D->GetDesc(&desc);
		texture2d->Implementation->Description = desc;

		return texture2d;
	}

	sptr<Texture2D> Texture2D::FromStream(GraphicsDevice& device, std::vector<Byte> const& data)
	{
		auto _this = device.shared_from_this();
		auto texture2d = snew<Texture2D>(_this);
		comptr<ID3D11Resource> resource = nullptr;

		auto hr = DirectX::CreateWICTextureFromMemory(
			device.Implementation->Device.Get(),
			device.Implementation->Context.Get(),
			data.data(),
			data.size(),
			resource.GetAddressOf(),
			texture2d->Implementation->ShaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr))
		{
			return nullptr;
		}
		
		hr = resource->QueryInterface(IID_ID3D11Texture2D, (void**)texture2d->Implementation->Texture2D.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		texture2d->Implementation->Texture2D->GetDesc(&desc);
		texture2d->Implementation->Description = desc;

		return texture2d;
	}	
}