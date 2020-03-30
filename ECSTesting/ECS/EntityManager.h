#pragma once

#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Component.h"

namespace Vofog{

	typedef size_t ComponentID;
	typedef size_t EntityID;

	class EntityManager{
	private:

		std::unordered_map<EntityID, std::unordered_map<ComponentID, std::shared_ptr<Component>>> _data;
		std::vector<EntityID> _ids;
		std::vector<Vofog::System> _systems;
	public:

		EntityManager() = default;

		void addEntity(EntityID id){
			_data[id] = std::unordered_map<ComponentID, std::shared_ptr<Component>>();
			_ids.push_back(id);
		}
		template<typename T>
		void attachComponent(const Entity& entity, std::shared_ptr<T> component){
			_data[entity.getID()][type_id<T>()] = component;
		}

		template<typename T, typename... Types>
		void attachComponent(const Entity& entity, std::shared_ptr<T> component, std::shared_ptr<Types> ... types){
			_data[entity.getID()][type_id<T>()] = component;
			attachComponent(entity, types...);
		}
	

		template<typename T>
		std::shared_ptr<T> getComponent(const Entity& entity){
			return std::dynamic_pointer_cast<T>(_data[entity.getID()][type_id<T>()]);
		}
		template<typename T>
		std::shared_ptr<T> getComponent(EntityID id){
			return std::dynamic_pointer_cast<T>(_data[id][type_id<T>()]);
		}

		template<typename T>
		std::vector<EntityID> getAssociatedEntities(){
			std::vector<EntityID> associatedIDs;
			for(int i = 0; i < _ids.size(); i++){
				if(_data[_ids[i]][type_id<T>()] != nullptr)
					associatedIDs.push_back(_ids[i]);
			}
			return associatedIDs;
		}

		template<typename T, typename N>
		std::vector<EntityID> getAssociatedEntities(){
			std::vector<EntityID> associatedIDs;
			for(int i = 0; i < _ids.size(); i++){
				if(_data[_ids[i]][type_id<T>()] != nullptr)
					for(int k = 0; k < _ids.size(); k++){
						if(_data[_ids[k]][type_id<N>()] != nullptr)
							associatedIDs.push_back(_ids[i]);
					}
			}
			return associatedIDs;
		}


	
	
	};








}