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
	
	public:

		EntityManager() = default;

		void addEntity(EntityID id){
			_data[id] = std::unordered_map<ComponentID, std::shared_ptr<Component>>();
		}
		template<typename T>
		void attachComponent(const Entity& entity, std::shared_ptr<T> component){
			_data[entity.getID()][type_id<T>()] = component;
		}
	

		template<typename T>
		std::shared_ptr<T> getComponent(const Entity& entity){
			return std::dynamic_pointer_cast<T>(_data[entity.getID()][type_id<T>()]);
		}
	
	
	};








}