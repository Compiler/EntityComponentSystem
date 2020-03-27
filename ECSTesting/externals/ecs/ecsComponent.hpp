#pragma once

#include "vector"
#include <tuple>

struct BaseECSComponent;
typedef void* EntityHandle;
typedef int (*ECSComponentCreateFunction)(std::vector<char>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);
#define NULL_ENTITY_HANDLE nullptr

struct BaseECSComponent
{
public:
	static int registerComponentType(ECSComponentCreateFunction createfn,
			ECSComponentFreeFunction freefn, size_t size);
	EntityHandle entity = NULL_ENTITY_HANDLE;

	inline static ECSComponentCreateFunction getTypeCreateFunction(int id)
	{
		return std::get<0>((*componentTypes)[id]);
	}

	inline static ECSComponentFreeFunction getTypeFreeFunction(int id)
	{
		return std::get<1>((*componentTypes)[id]);
	}

	inline static size_t getTypeSize(int id)
	{
		return std::get<2>((*componentTypes)[id]);
	}

	inline static bool isTypeValid(int id)
	{
		return id < componentTypes->size();
	}
private:
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t> >* componentTypes;
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;
	static const int ID;
	static const size_t SIZE; 
};

template<typename Component>
int ECSComponentCreate(std::vector<char>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	int index = memory.size();
	memory.resize(index+Component::SIZE);
	Component* component = new(&memory[index])Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const int ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

//BEGIN EXAMPLE CODE
struct TestComponent : public ECSComponent<TestComponent>
{
	float x;
	float y;
};

