#include "graphics/depthstencilstate.hpp"
#include "platform-dx/dxheaders.hpp"
#include "platform-dx/implementations.hpp"

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

		_description.StencilReadMask = 0;
		_description.StencilWriteMask = 0;
		_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return _description;
	}

	DepthStencilState::DepthStencilState() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
		impl->dxDescription = defaultDesc();
	}

	DepthStencilState::DepthStencilState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
		impl->dxDescription = defaultDesc();
	}

	DepthStencilState::~DepthStencilState() {
		impl = nullptr;
	}

	bool DepthStencilState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->impl->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->dxDepthStencil) {
			impl->dxDepthStencil->Release();
			impl->dxDepthStencil = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateDepthStencilState(
			&impl->dxDescription,
			&impl->dxDepthStencil);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool DepthStencilState::Apply(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->impl->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!impl->dxDepthStencil) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}

		m_device->impl->_context->OMSetDepthStencilState(impl->dxDepthStencil, 0);

		return true;
	}

	uptr<DepthStencilState> DepthStencilState::None() {
		auto stencil = uNew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = false;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::Default() {
		auto stencil = uNew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = true;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::DepthRead() {
		auto stencil = uNew<DepthStencilState>();
		stencil->impl->dxDescription.DepthEnable = true;
		stencil->impl->dxDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	void DepthStencilState::DepthEnabled(bool value) {
		impl->dxDescription.DepthEnable = value;
	}

	void DepthStencilState::DepthWriteEnabled(bool value) {
		impl->dxDescription.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(value);
	}

	void DepthStencilState::DepthCompareFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::StencilEnabled(bool value) {
		impl->dxDescription.StencilEnable = value;
	}

	void DepthStencilState::StencilReadMask(int value) {
		impl->dxDescription.StencilReadMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilWriteMask(Int value) {
		impl->dxDescription.StencilWriteMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilFrontFacePass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFrontFaceFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFrontFaceDepthFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFrontFaceCompare(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::StencilBackFacePass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilBackFaceFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilBackFaceDepthFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilBackFaceCompare(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		impl->dxDescription.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	bool DepthStencilState::DepthEnabled() const {
		return impl->dxDescription.DepthEnable;
	}

	bool DepthStencilState::DepthWriteEnabled() const {
		return static_cast<bool>(impl->dxDescription.DepthWriteMask);
	}

	ComparisonFunction DepthStencilState::DepthCompareFunction() const {
		const auto _value = static_cast<int>(impl->dxDescription.DepthFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	bool DepthStencilState::StencilEnabled() const {
		return impl->dxDescription.StencilEnable;
	}

	Int DepthStencilState::StencilReadMask() const {
		return static_cast<int>(impl->dxDescription.StencilReadMask);
	}

	Int DepthStencilState::StencilWriteMask() const {
		return static_cast<int>(impl->dxDescription.StencilWriteMask);
	}

	StencilOperation DepthStencilState::StencilFrontFacePass() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilFrontFaceFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilFrontFaceDepthFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::StencilFrontFaceCompare() const {
		const auto _value = static_cast<int>(impl->dxDescription.FrontFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	StencilOperation DepthStencilState::StencilBackFacePass() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilBackFaceFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilBackFaceDepthFail() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::StencilBackFaceCompare() const {
		const auto _value = static_cast<int>(impl->dxDescription.BackFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}
}