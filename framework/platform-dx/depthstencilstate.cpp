#include "xna/graphics/depthstencilstate.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	static D3D11_DEPTH_STENCIL_DESC defaultDesc() {
		D3D11_DEPTH_STENCIL_DESC _description{};
		_description.DepthEnable = true;
		_description.StencilEnable = true;
		_description.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		_description.StencilReadMask = IntMaxValue;
		_description.StencilWriteMask = IntMaxValue;
		_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return _description;
	}

	DepthStencilState::DepthStencilState() : GraphicsResource(nullptr) {
		impl = unew<PlatformImplementation>();
		impl->dxDescription = defaultDesc();
	}

	DepthStencilState::DepthStencilState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
		impl->dxDescription = defaultDesc();
	}	

	bool DepthStencilState::Initialize()
	{
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
		}

		if (impl->dxDepthStencil) {
			impl->dxDepthStencil = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateDepthStencilState(
			&impl->dxDescription,
			impl->dxDepthStencil.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	bool DepthStencilState::Apply()
	{
		if (!m_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::InvalidOperation);
		}

		if (!impl->dxDepthStencil) {
			Exception::Throw(ExMessage::UnintializedComponent);
		}

		m_device->impl->_context->OMSetDepthStencilState(impl->dxDepthStencil.Get(), 0);

		return true;
	}

	uptr<DepthStencilState> DepthStencilState::None() {
		auto stencil = unew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = false;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::Default() {
		auto stencil = unew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = true;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::DepthRead() {
		auto stencil = unew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = true;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	void DepthStencilState::DepthBufferEnable(bool value) {
		impl->dxDescription.DepthEnable = value;
	}

	void DepthStencilState::DepthBufferWriteEnable(bool value) {
		impl->dxDescription.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(value);
	}

	void DepthStencilState::DepthBufferFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::StencilEnable(bool value) {
		impl->dxDescription.StencilEnable = value;
	}

	void DepthStencilState::StencilMask(int value) {
		impl->dxDescription.StencilReadMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilWriteMask(Int value) {
		impl->dxDescription.StencilWriteMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilPass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilDepthBufferFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilPass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilDepthBufferFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	bool DepthStencilState::DepthBufferEnable() const {
		return impl->dxDescription.DepthEnable;
	}

	bool DepthStencilState::DepthBufferWriteEnable() const {
		return static_cast<bool>(impl->dxDescription.DepthWriteMask);
	}

	ComparisonFunction DepthStencilState::DepthBufferFunction() const {
		const auto _value = static_cast<int>(impl->dxDescription.DepthFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	bool DepthStencilState::StencilEnable() const {
		return impl->dxDescription.StencilEnable;
	}

	Int DepthStencilState::StencilMask() const {
		return static_cast<int>(impl->dxDescription.StencilReadMask);
	}

	Int DepthStencilState::StencilWriteMask() const {
		return static_cast<int>(impl->dxDescription.StencilWriteMask);
	}

	StencilOperation DepthStencilState::StencilPass() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilDepthBufferFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::StencilFunction() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilPass() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilDepthBufferFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::CounterClockwiseStencilFunction() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}
}