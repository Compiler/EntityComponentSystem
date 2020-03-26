#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "Component.h"
#include "System.h"



namespace Vofog{

	class ECS{

	public:

		ECS(){}

		
		//entity methods
		EntityHandle createEntity(Component* components, const int* componentIDs, size_t numComponents);
		void removeEntity(EntityHandle handle);


		//System methods
		inline void addSystem(System& system){
			_systems.emplace_back(&system);
		}
		void removeSystem(System& system);
		void updateSystems();



		//Component methods
		template<class Comp>
		void addComponent(EntityHandle entity, Comp* component);



		template<class Comp>
		void removeComponent(EntityHandle entity);

		template<class Comp>
		void getComponent(EntityHandle entity);


		~ECS();

	private:

		std::vector<std::shared_ptr<System>> _systems;
		std::map<int, std::vector<uint8_t>> _components;
		std::vector<std::pair<int, std::vector<std::pair<int, int>>>*> _entities;

		inline std::pair<int, std::vector<std::pair<int, int>>>* handleToRawType(EntityHandle handle){
			//(std::pair<int, std::vector<std::pair<int, int>>>*) handle;
			return (std::pair<int, std::vector<std::pair<int, int>>>*) handle;

		}

		inline int handleToEntityIndex(EntityHandle handle){
			return handleToRawType(handle)->first;
		}

		inline std::vector<std::pair<int, int> >& handleToEntity(EntityHandle handle){
			return handleToRawType(handle)->second;
		}

	};

}