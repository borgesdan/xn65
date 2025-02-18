#include "xna/game/servicecontainer.hpp"

namespace xna {
	void GameServiceContainer::AddService(csharp::Type const& type, std::any& provider)
	{
		auto hashCode = type.GetHashCode();
		services.insert({ hashCode, provider });
	}

	std::any GameServiceContainer::GetService(csharp::Type const& serviceType)
	{
		auto hashCode = serviceType.GetHashCode();

		return services.contains(hashCode)
			? services[hashCode]
			: std::any();
	}

	void GameServiceContainer::RemoveService(csharp::Type const& type) {
		auto hashCode = type.GetHashCode();

		if (services.contains(hashCode))
			services.erase(hashCode);

	}
}