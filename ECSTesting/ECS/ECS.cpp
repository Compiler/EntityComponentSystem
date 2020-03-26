#include "ECS.h"


namespace Vofog{






	EntityHandle ECS::createEntity(Component* components, const int* componentIDs, size_t numComponents){
		std::pair<int, std::vector<std::pair<int, int>>>* newEntity = new std::pair<int, std::vector<std::pair<int, int>>>();
		EntityHandle handle = (EntityHandle)newEntity;

		for(int i = 0; i < numComponents; i++){
			//assert componentID is valid



		}

		newEntity->first = _entities.size();
		_entities.push_back(newEntity);
		return handle;
	}






	ECS::~ECS(){


		for(std::map<int, std::vector<uint8_t>>::iterator it = _components.begin(); it != _components.end(); ++it){
			size_t typeSize = Component::getTypeSize(it->first);
			ComponentFreeFunction freeFunction = Component::getTypeFreeFunction(it->first);
			for(int i = 0; i < it->second.size(); i += typeSize){
				freeFunction((Component*)(&it->second[i]));
			}
		}
		for(int i = 0; i < _entities.size(); i++)
			delete _entities[i];




		
	
	
	}


}