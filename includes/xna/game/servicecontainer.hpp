#ifndef XNA_GAME_SERVICECONTAINER_HPP
#define XNA_GAME_SERVICECONTAINER_HPP

#include "../default.hpp"
#include "csharp/service.hpp"
#include <map>

namespace xna {
	//A collection of game services. 
	class GameServiceContainer : public csharp::IServiceProvider {
	public:
		//Adds a service to the GameServiceContainer.
		void AddService(csharp::Type const& type, std::any& provider);

		// Inherited via IServiceProvider
		std::any GetService(csharp::Type const& serviceType) override;

		//Removes the object providing a specified service. 
		void RemoveService(csharp::Type const& type);

	private:
		std::map<size_t, std::any> services;
	};
}

#endif