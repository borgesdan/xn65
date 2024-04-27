#ifndef XNA_GAME_COMPONENT_HPP
#define XNA_GAME_COMPONENT_HPP

#include "../default.hpp"
#include "time.hpp"
#include <algorithm>

namespace xna {
	class IGameComponent {
	public:
		virtual void Initialize() = 0;
		virtual GameComponentType Type() = 0;		
	};

	class IUpdateable {
	public:
		virtual bool Enabled() const = 0;
		virtual Int UpdateOrder() const = 0;
		virtual void Update(GameTime const& gameTime) = 0;
	};

	class IDrawable
	{
	public:
		virtual bool Visible() const = 0;
		virtual Int DrawOrder() const = 0;
		virtual void Draw(GameTime const& gameTime) = 0;
	};

	class GameComponent : public IGameComponent, public IUpdateable {
	public:
		virtual ~GameComponent() {}
		
		virtual void Initialize() override {}
		
		constexpr bool Enabled() const override {
			return enabled;
		}

		constexpr void Enabled(bool value) {
			enabled = value;
		}

		constexpr Int UpdateOrder() const override {
			return updateOrder;
		}

		constexpr void UpdateOrder(Int value) {
			updateOrder = value;
		}

		virtual constexpr GameComponentType Type() override {
			return GameComponentType::Updatable;
		}

		virtual void Update(GameTime const& gameTime) override{}		

	private:
		bool enabled{ false };
		Int updateOrder{ 0 };
	};

	class DrawableGameComponent : public GameComponent, public IDrawable {
	public:
		virtual ~DrawableGameComponent() {}		
		
		constexpr bool Visible() const override {
			return visible;
		}

		constexpr Int DrawOrder() const override {
			return drawOrder;
		}
		
		virtual void Draw(GameTime const& gameTime) override {
		}
		
		constexpr void Visible(bool value) {
			visible = value;
		}

		constexpr void DrawOrder(Int value) {
			drawOrder = value;
		}

		inline virtual void Initialize() override{
			if(!initialized) LoadContent();
			initialized = true;
		}

		virtual constexpr GameComponentType Type() override {
			return GameComponentType::Drawable;
		}

	protected:
		virtual void LoadContent() {}
		virtual void UnloadContent() {}

	private:
		bool visible{ false };
		bool initialized{ false };
		Int drawOrder{ 0 };
	};

	class GameComponentCollection {
	public:
		void InsertItem(size_t index, sptr<IGameComponent> const& item) {
			const auto it = components.begin();
			components.insert(it + index, item);

			std::sort(components.begin(), components.end(), UpdateOrderComparer);
		}

		void RemoveItem(size_t index) {
			if (index >= components.size())
				return;						

			const auto it = components.begin();
			components.erase(it + index);

			std::sort(components.begin(), components.end(), UpdateOrderComparer);
		}

		void ClearItems() {
			components.clear();			
		}

		void Add(sptr<IGameComponent> const& item) {
			components.push_back(item);	
			std::sort(components.begin(), components.end(), UpdateOrderComparer);
		}

		constexpr size_t Count() const {
			return components.size();
		}

		sptr<IGameComponent> operator[](size_t index) const {
			if (index >= components.size())
				return nullptr;

			return components[index];
		}

		sptr<IGameComponent> At(size_t index) const {
			if (index >= components.size())
				return nullptr;

			return components[index];
		}

		static bool UpdateOrderComparer(sptr<IGameComponent> const& x, sptr<IGameComponent> const& y) {
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

		static bool DrawOrderComparer(sptr<IGameComponent> const& x, sptr<IGameComponent> const& y) {
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

	public:
		std::vector<sptr<IGameComponent>> components;			
	};	
}

#endif