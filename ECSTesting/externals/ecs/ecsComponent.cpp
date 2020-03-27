#include "ecsComponent.hpp"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> >* BaseECSComponent::componentTypes;

int BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn,
			ECSComponentFreeFunction freefn, size_t size)
{
	if(componentTypes == nullptr) {
		componentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> >();
	}
	int componentID = componentTypes->size();
	componentTypes->push_back(std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>(
				createfn, freefn, size));
	return componentID;
}
