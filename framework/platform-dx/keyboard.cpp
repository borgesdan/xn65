#include "xna/input/keyboard.hpp"
#include "xna/platform-dx/implementations.hpp"

namespace xna {
	KeyboardState Keyboard::GetState() {
		if (!impl || !impl->_dxKeyboard)
			return KeyboardState();

		const auto state = Keyboard::impl->_dxKeyboard->GetState();
		auto ptr = reinterpret_cast<const uint32_t*>(&state);
		const auto xnaState = reinterpret_cast<const KeyboardState*>(ptr);

		return *xnaState;
	}

	void Keyboard::Initialize() {
		impl = uNew<PlatformImplementation>();
		impl->_dxKeyboard = uNew<DirectX::Keyboard>();
	}

	bool Keyboard::IsConnected() {
		if (!impl || !impl->_dxKeyboard)
			return false;

		return impl->_dxKeyboard->IsConnected();
	}
}