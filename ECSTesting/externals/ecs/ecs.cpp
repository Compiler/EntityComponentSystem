#include "ecs.hpp"
#include <algorithm>

ECS::~ECS()
{
	for(std::map<int, std::vector<char>>::iterator it = components.begin(); it != components.end(); ++it) {
		size_t typeSize = BaseECSComponent::getTypeSize(it->first);
		ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(it->first);
		for(int i = 0; i < it->second.size(); i += typeSize) {
			freefn((BaseECSComponent*)&it->second[i]);
		}
	}

	for(int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
}

EntityHandle ECS::makeEntity(BaseECSComponent** entityComponents, const int* componentIDs, size_t numComponents)
{
	std::pair<int, std::vector<std::pair<int, int> > >* newEntity = new std::pair<int, std::vector<std::pair<int, int> > >();
	EntityHandle handle = (EntityHandle)newEntity;
	for(int i = 0; i < numComponents; i++) {
		if(!BaseECSComponent::isTypeValid(componentIDs[i])) {
			//DEBUG_LOG("ECS", LOG_ERROR, "'%u' is not a valid component type.", componentIDs[i]);
			delete newEntity;
			return NULL_ENTITY_HANDLE;
		}

		addComponentInternal(handle, newEntity->second, componentIDs[i], entityComponents[i]);
	}

	newEntity->first = entities.size();
	entities.push_back(newEntity);

	for(int i = 0; i < listeners.size(); i++) {
		bool isValid = true;
		if(listeners[i]->shouldNotifyOnAllEntityOperations()) {
			listeners[i]->onMakeEntity(handle);
		} else {
			for(int j = 0; j < listeners[i]->getComponentIDs().size(); j++) {
				bool hasComponent = false;
				for(int k = 0; k < numComponents; k++) {
					if(listeners[i]->getComponentIDs()[j] == componentIDs[k]) {
						hasComponent = true;
						break;
					}
				}
				if(!hasComponent) {
					isValid = false;
					break;
				}
			}
			if(isValid) {
				listeners[i]->onMakeEntity(handle);
			}
		}
	}

	return handle;
}

void ECS::removeEntity(EntityHandle handle)
{
	std::vector<std::pair<int, int> >& entity = handleToEntity(handle);

	for(int i = 0; i < listeners.size(); i++) {
		const std::vector<int>& componentIDs = listeners[i]->getComponentIDs();
		bool isValid = true;
		if(listeners[i]->shouldNotifyOnAllEntityOperations()) {
			listeners[i]->onRemoveEntity(handle);
		} else {
			for(int j = 0; j < componentIDs.size(); j++) {
				bool hasComponent = false;
				for(int k = 0; k < entity.size(); k++) {
					if(componentIDs[j] == entity[k].first) {
						hasComponent = true;
						break;
					}
				}
				if(!hasComponent) {
					isValid = false;
					break;
				}
			}
			if(isValid) {
				listeners[i]->onRemoveEntity(handle);
			}
		}
	}
	
	for(int i = 0; i < entity.size(); i++) {
		deleteComponent(entity[i].first, entity[i].second);
	}

	int destIndex = handleToEntityIndex(handle);
	int srcIndex = entities.size() - 1;
	delete entities[destIndex];
	entities[destIndex] = entities[srcIndex];
	entities[destIndex]->first = destIndex;
	entities.pop_back();
}

void ECS::addComponentInternal(EntityHandle handle, std::vector<std::pair<int, int> >& entity, int componentID, BaseECSComponent* component)
{
	ECSComponentCreateFunction createfn = BaseECSComponent::getTypeCreateFunction(componentID);
	std::pair<int, int> newPair;
	newPair.first = componentID;
	newPair.second = createfn(components[componentID], handle, component);
	entity.push_back(newPair);
}

void ECS::deleteComponent(int componentID, int index)
{
	std::vector<char>& array = components[componentID];
	ECSComponentFreeFunction freefn = BaseECSComponent::getTypeFreeFunction(componentID);
	size_t typeSize = BaseECSComponent::getTypeSize(componentID);
	int srcIndex = array.size() - typeSize;

	BaseECSComponent* destComponent = (BaseECSComponent*)&array[index];
	BaseECSComponent* srcComponent = (BaseECSComponent*)&array[srcIndex];
	freefn(destComponent);

	if(index == srcIndex) {
		array.resize(srcIndex);
		return;
	}
	memcpy(destComponent, srcComponent, typeSize);

	std::vector<std::pair<int, int> >& srcComponents = handleToEntity(srcComponent->entity);
	for(int i = 0; i < srcComponents.size(); i++) {
		if(componentID == srcComponents[i].first && srcIndex == srcComponents[i].second) {
			srcComponents[i].second = index;
			break;
		}
	}

	array.resize(srcIndex);
}

bool ECS::removeComponentInternal(EntityHandle handle, int componentID)
{
	std::vector<std::pair<int, int> >& entityComponents = handleToEntity(handle);
	for(int i = 0; i < entityComponents.size(); i++) {
		if(componentID == entityComponents[i].first) {
			deleteComponent(entityComponents[i].first, entityComponents[i].second);
			int srcIndex = entityComponents.size()-1;
			int destIndex = i;
			entityComponents[destIndex] = entityComponents[srcIndex];
			entityComponents.pop_back();
			return true;
		}
	}
	return false;
}

BaseECSComponent* ECS::getComponentInternal(std::vector<std::pair<int, int> >& entityComponents, std::vector<char>& array, int componentID)
{
	for(int i = 0; i < entityComponents.size(); i++) {
		if(componentID == entityComponents[i].first) {
			return (BaseECSComponent*)&array[entityComponents[i].second];
		}
	}
	return nullptr;
}

void ECS::updateSystems(ECSSystemList& systems, float delta)
{
	std::vector<BaseECSComponent*> componentParam;
	std::vector<std::vector<char>*> componentArrays;
	for(int i = 0; i < systems.size(); i++) {
		const std::vector<int>& componentTypes = systems[i]->getComponentTypes();
		if(componentTypes.size() == 1) {
			size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[0]);
			std::vector<char>& array = components[componentTypes[0]];
			for(int j = 0; j < array.size(); j += typeSize) {
				BaseECSComponent* component = (BaseECSComponent*)&array[j];
				systems[i]->updateComponents(delta, &component);
			}
		} else {
			updateSystemWithMultipleComponents(i, systems, delta, componentTypes, componentParam, componentArrays);
		}
	}
}

int ECS::findLeastCommonComponent(const std::vector<int>& componentTypes, const std::vector<int>& componentFlags)
{
	int minSize = (int)0;
	int minIndex = (int)0;//-1 -> 0
	for(int i = 0; i < componentTypes.size(); i++) {
		if((componentFlags[i] & BaseECSSystem::FLAG_OPTIONAL) != 0) {
			continue;
		}
		size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[i]);
		int size = components[componentTypes[i]].size()/typeSize;
		if(size <= minSize) {
			minSize = size;
			minIndex = i;
		}
	}

	return minIndex;
}
#include <math.h>
void ECS::updateSystemWithMultipleComponents(int index, ECSSystemList& systems, float delta,
		const std::vector<int>& componentTypes, std::vector<BaseECSComponent*>& componentParam,
		std::vector<std::vector<char>*>& componentArrays)
{
	const std::vector<int>& componentFlags = systems[index]->getComponentFlags();

	componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));
	for(int i = 0; i < componentTypes.size(); i++) {
		componentArrays[i] = &components[componentTypes[i]];
	}
	int minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

	size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[minSizeIndex]);
	std::vector<char>& array = *componentArrays[minSizeIndex];
	for(int i = 0; i < array.size(); i += typeSize) {
		componentParam[minSizeIndex] = (BaseECSComponent*)&array[i];
		std::vector<std::pair<int, int> >& entityComponents =
			handleToEntity(componentParam[minSizeIndex]->entity);

		bool isValid = true;
		for(int j = 0; j < componentTypes.size(); j++) {
			if(j == minSizeIndex) {
				continue;
			}

			componentParam[j] = getComponentInternal(entityComponents,
					*componentArrays[j], componentTypes[j]);
			if(componentParam[j] == nullptr && (componentFlags[j] & BaseECSSystem::FLAG_OPTIONAL) == 0) {
				isValid = false;
				break;
			}
		}

		if(isValid) {
			systems[index]->updateComponents(delta, &componentParam[0]);
		}
	}
}

