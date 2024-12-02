#include "pipeline/serialization.hpp"

namespace xna {
	std::shared_ptr<IntermediateSerializer> IntermediateSerializer::SingletonInstance() {
		if (singletonInstance == nullptr)
			singletonInstance = std::make_shared<IntermediateSerializer>();

		return singletonInstance;
	}