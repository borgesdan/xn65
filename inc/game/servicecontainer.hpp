#ifndef XNA_GAME_SERVICECONTAINER_HPP
#define XNA_GAME_SERVICECONTAINER_HPP

#include "../default.hpp"
#include "../csharp/service.hpp"
#include <map>

namespace xna {
	class GameServiceContainer : public IServiceProvider {
	public:
		void AddService(Type& type, std::any& provider);

		// Inherited via IServiceProvider
		std::any GetService(Type& serviceType) override;

	private:
		std::map<size_t, std::any> services;
	};
}

#endif