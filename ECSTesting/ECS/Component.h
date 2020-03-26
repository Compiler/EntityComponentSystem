#pragma once

#include <iostream>
#include <vector>
#include "../GameObject.h"
#include <tuple>
namespace Vofog{


	struct Component;
	typedef void* EntityHandle;
	typedef int(*ComponentCreateFunction)(std::vector<int>& memory, EntityHandle entity, Component* comp);
	typedef void(*ComponentFreeFunction)(Component* comp);
	#define NULL_ENTITY_HANDLE nullptr

	class Component{
	
	public:
		static int registerComponentType(ComponentCreateFunction createFunction, ComponentFreeFunction freeFunction, size_t size);
		EntityHandle entity = NULL_ENTITY_HANDLE;
	
		inline static ComponentCreateFunction getTypeCreateFunction(int id){
			return std::get<0>(_componentTypes[id]);
		}

		inline static ComponentFreeFunction getTypeFreeFunction(int id){
			return std::get<1>(_componentTypes[id]);
		}


		inline static int getTypeSize(int id){
			return std::get<2>(_componentTypes[id]);
		}

	private:
		static std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>> _componentTypes;
	};


	template<typename T>
	struct BaseComponent : public Component{

		static const ComponentCreateFunction CREATE_FUNCTION;
		static const ComponentFreeFunction FREE_FUNCTION;

		static const int ID;
		static const size_t SIZE;

	};

	template<typename Comp>
	int componentCreate(std::vector<int>& memory, EntityHandle entity, Component* comp){
		int index = memory.size();
		memory.resize(index + Comp::SIZE);
		Comp* component = new(&memory[index]) Comp(*(Comp*)comp);//new(x), x = where to construct new component
		component->entity = entity;
		return index;

	}

	template<typename Comp>
	void componentFree(Component* comp){
		Comp* component = (Comp*)comp;
		component->~Comp();
	}
	
	template<typename T>
	const int BaseComponent<T>::ID(Component::registerComponentType(componentCreate<T>, componentFree<T>, sizeof(T)));
	
	template<typename T>
	const size_t BaseComponent<T>::SIZE(sizeof(T));

	template<typename T>
	const ComponentCreateFunction BaseComponent<T>::CREATE_FUNCTION(componentCreate<T>);

	template<typename T>
	const ComponentFreeFunction BaseComponent<T>::FREE_FUNCTION(componentFree<T>);








	struct PositionComponent: public BaseComponent<PositionComponent>{
		float x, y;
	};
	struct HealthComponent: public BaseComponent<HealthComponent>{
		float health;
	};




}