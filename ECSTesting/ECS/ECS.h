#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "Component.h"
#include "System.h"



namespace Vofog{

	class ECS{
	private:

		std::vector<System*> _systems;
		std::map<int, std::vector<int>> _components;
		std::vector < std::pair<int, std::vector<std::pair<int, int>>*>> _entities;

		inline handleToRawType(EntityHandle handle){



		}

	};

}