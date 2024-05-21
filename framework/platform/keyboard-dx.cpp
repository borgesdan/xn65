#include "input/keyboard.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	KeyboardState Keyboard::GetState() {
		if (!impl->_dxKeyboard)
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
}