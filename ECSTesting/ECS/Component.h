#pragma once

#include <iostream>
#include <vector>
#include "../GameObject.h"
#include <unordered_map>

namespace Vofog{
	

	template<typename T>
	struct type{ static void id(){} };

	template<typename T>
	size_t type_id(){ return reinterpret_cast<size_t>(&type<T>::id); }
	#define MAX_COMPONENTS 64

	class Component{
	protected:
		std::string DEBUG_NAME = "Component";
	public:
		void printDebugName(){ std::cout << DEBUG_NAME << "\n"; }


		virtual ~Component(){}
	};



	struct RenderableComponent : public Component{
		RenderableComponent(){ DEBUG_NAME = "RenderableComponent"; }
		char data;
	};

	struct TransformComponent: public Component{
		TransformComponent(){ DEBUG_NAME = "TransformComponent"; }
		float xPos, yPos;
	};

	struct InputComponent: public Component{
		//static const ComponentID ID = Component::getID<InputComponent>();
		InputComponent(){ DEBUG_NAME = "InputComponent"; }
		std::vector<char> dataQueue;
	};

}