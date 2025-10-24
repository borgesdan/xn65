#ifndef XNA_IREFERENCE_HPP
#define XNA_IREFERENCE_HPP

namespace Xna {
	template <typename T>
	struct IReference {
	protected:
		std::shared_ptr<T> m_reference = nullptr;
	};
}

#endif