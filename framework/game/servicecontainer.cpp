#include "servicecontainer.hpp"

namespace xna {
	void GameServiceContainer::AddService(Type& type, std::any& provider)
	{
		auto hashCode = type.GetHashCode();
		services.insert({ hashCode, provider });
	}

	std::any GameServiceContainer::GetService(Type& serviceType)
	{
		auto hashCode = serviceType.GetHashCode();

		return services.contains(hashCode)
			? services[hashCode]
			: std::any();
	}
}