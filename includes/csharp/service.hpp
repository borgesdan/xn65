#ifndef XNA_CSHARP_SERVICE_HPP
#define XNA_CSHARP_SERVICE_HPP

#include "type.hpp"
#include <any>

namespace csharp {
	//A simplified port of the System.IServiceProvider
	//Defines a mechanism for retrieving a service object; that is, an object that provides custom support to other objects.
	class IServiceProvider {
	public:
		//Gets the service object of the specified type.
		virtual std::any GetService(Type const& serviceType) = 0;
	};
}

#endif