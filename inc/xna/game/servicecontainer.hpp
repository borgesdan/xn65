#ifndef XNA_GAME_SERVICECONTAINER_HPP
#define XNA_GAME_SERVICECONTAINER_HPP

#include "../default.hpp"
#include "../csharp/service.hpp"
#include <map>

namespace xna {
	//A collection of game services. 
	class GameServiceContainer : public IServiceProvider {
	public:
		//Adds a service to the GameServiceContainer.
		void AddService(Type& type, std::any& provider);

		// Inherited via IServiceProvider
		std::any GetService(Type& serviceType) override;

		//Removes the object providing a specified service. 
		void RemoveService(Type& type);

	private:
		std::map<size_t, std::any> services;
	};
}

#endif