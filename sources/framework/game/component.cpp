#include "xna/game/component.hpp"

namespace xna {
	sptr<IGameComponent> GameComponentCollection::operator[](size_t index) const
	{
		if (index >= components.size())
			return nullptr;

		return components[index];
	}

	sptr<IGameComponent> GameComponentCollection::At(size_t index) const
	{
		if (index >= components.size())
			return nullptr;

		return components[index];
	}

	bool GameComponentCollection::UpdateOrderComparer(sptr<IGameComponent> const& x, sptr<IGameComponent> const& y)
	{
		auto comp1 = std::reinterpret_pointer_cast<GameComponent>(x);
		auto comp2 = std::reinterpret_pointer_cast<GameComponent>(y);

		if (!comp1 && !comp2)
			return false;

		if (!comp1)
			return true;

		if (!comp2)
			return false;

		return comp1->UpdateOrder() < comp2->UpdateOrder();
	}

	bool GameComponentCollection::DrawOrderComparer(sptr<IGameComponent> const& x, sptr<IGameComponent> const& y)
	{
		auto comp1 = std::reinterpret_pointer_cast<DrawableGameComponent>(x);
		auto comp2 = std::reinterpret_pointer_cast<DrawableGameComponent>(y);

		if (!comp1 && !comp2)
			return false;

		if (!comp1)
			return true;

		if (!comp2)
			return false;

		return comp1->DrawOrder() < comp2->DrawOrder();
	}
}