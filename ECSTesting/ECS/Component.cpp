#include "Component.h"


std::vector<std::tuple<Vofog::ComponentCreateFunction, Vofog::ComponentFreeFunction, size_t>> Vofog::Component::_componentTypes;

int Vofog::Component::registerComponentType(ComponentCreateFunction createFunction, ComponentFreeFunction freeFunction, size_t size){
	int id = _componentTypes.size();
	_componentTypes.push_back(std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(createFunction, freeFunction, size));
	return id++;
}