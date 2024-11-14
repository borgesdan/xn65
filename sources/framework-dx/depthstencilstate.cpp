#include "xna/graphics/depthstencilstate.hpp"
#include "xna-dx/framework.hpp"

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

		_description.StencilReadMask = static_cast<UINT8>(IntMaxValue);
		_description.StencilWriteMask = static_cast<UINT8>(IntMaxValue);
		_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return _description;
	}

	DepthStencilState::DepthStencilState() : GraphicsResource(nullptr) {
		Implementation = unew<DepthStencilStateImplementation>();
		Implementation->Description = defaultDesc();
	}

	DepthStencilState::DepthStencilState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		Implementation = unew<DepthStencilStateImplementation>();
		Implementation->Description = defaultDesc();
	}	

	bool DepthStencilState::Initialize()
	{
		if (!m_device || !m_device->Implementation->Device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (Implementation->DepthStencil) {
			Implementation->DepthStencil = nullptr;
		}

		const auto hr = m_device->Implementation->Device->CreateDepthStencilState(
			&Implementation->Description,
			Implementation->DepthStencil.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		return true;
	}

	bool DepthStencilState::Apply()
	{
		if (!m_device || !m_device->Implementation->Context) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		if (!Implementation->DepthStencil) {
			Initialize();
		}

		m_device->Implementation->Context->OMSetDepthStencilState(Implementation->DepthStencil.Get(), 0);

		return true;
	}

	uptr<DepthStencilState> DepthStencilState::None() {
		auto stencil = unew<DepthStencilState>();
		stencil->Implementation->Description.DepthEnable = false;
		stencil->Implementation->Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::Default() {
		auto stencil = unew<DepthStencilState>();
		stencil->Implementation->Description.DepthEnable = true;
		stencil->Implementation->Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		return stencil;
	}

	uptr<DepthStencilState> DepthStencilState::DepthRead() {
		auto stencil = unew<DepthStencilState>();
		stencil->Implementation->Description.DepthEnable = true;
		stencil->Implementation->Description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		return stencil;
	}

	void DepthStencilState::DepthBufferEnable(bool value) {
		Implementation->Description.DepthEnable = value;
	}

	void DepthStencilState::DepthBufferWriteEnable(bool value) {
		Implementation->Description.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(value);
	}

	void DepthStencilState::DepthBufferFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::StencilEnable(bool value) {
		Implementation->Description.StencilEnable = value;
	}

	void DepthStencilState::StencilMask(int value) {
		Implementation->Description.StencilReadMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilWriteMask(Int value) {
		Implementation->Description.StencilWriteMask = static_cast<UINT8>(value);
	}

	void DepthStencilState::StencilPass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilDepthBufferFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::StencilFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilPass(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilDepthBufferFail(StencilOperation value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(_value);
	}

	void DepthStencilState::CounterClockwiseStencilFunction(ComparisonFunction value) {
		const auto _value = static_cast<int>(value) + 1;
		Implementation->Description.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(_value);
	}

	bool DepthStencilState::DepthBufferEnable() const {
		return Implementation->Description.DepthEnable;
	}

	bool DepthStencilState::DepthBufferWriteEnable() const {
		return static_cast<bool>(Implementation->Description.DepthWriteMask);
	}

	ComparisonFunction DepthStencilState::DepthBufferFunction() const {
		const auto _value = static_cast<int>(Implementation->Description.DepthFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	bool DepthStencilState::StencilEnable() const {
		return Implementation->Description.StencilEnable;
	}

	Int DepthStencilState::StencilMask() const {
		return static_cast<int>(Implementation->Description.StencilReadMask);
	}

	Int DepthStencilState::StencilWriteMask() const {
		return static_cast<int>(Implementation->Description.StencilWriteMask);
	}

	StencilOperation DepthStencilState::StencilPass() const {
		const auto _value = static_cast<int>(Implementation->Description.FrontFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilFail() const {
		const auto _value = static_cast<int>(Implementation->Description.FrontFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::StencilDepthBufferFail() const {
		const auto _value = static_cast<int>(Implementation->Description.FrontFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::StencilFunction() const {
		const auto _value = static_cast<int>(Implementation->Description.FrontFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilPass() const {
		const auto _value = static_cast<int>(Implementation->Description.BackFace.StencilPassOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilFail() const {
		const auto _value = static_cast<int>(Implementation->Description.BackFace.StencilFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	StencilOperation DepthStencilState::CounterClockwiseStencilDepthBufferFail() const {
		const auto _value = static_cast<int>(Implementation->Description.BackFace.StencilDepthFailOp) - 1;
		return static_cast<StencilOperation>(_value);
	}

	ComparisonFunction DepthStencilState::CounterClockwiseStencilFunction() const {
		const auto _value = static_cast<int>(Implementation->Description.BackFace.StencilFunc) - 1;
		return static_cast<ComparisonFunction>(_value);
	}
}