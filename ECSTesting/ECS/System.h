#pragma once

#include "Component.h"


namespace Vofog{

	class System{

	private:  
		std::vector<int> _componentTypes;
	public:
		System(const std::vector<int>& componentTypes): _componentTypes(componentTypes){}
		virtual void updateComponents(Component** components){}
		const std::vector<int>& getComponentTypes()const{ return _componentTypes; }


	};







}