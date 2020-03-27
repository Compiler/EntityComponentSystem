#pragma once

#include "ecsComponent.hpp"
#include "ecsSystem.hpp"
#include <map>
#include <vector>

class ECSListener
{
public:
	virtual void onMakeEntity(EntityHandle handle) {}
	virtual void onRemoveEntity(EntityHandle handle) {}
	virtual void onAddComponent(EntityHandle handle, int id) {}
	virtual void onRemoveComponent(EntityHandle handle, int id) {}

	const std::vector<int>& getComponentIDs() { 
		return componentIDs;
	}

	inline bool shouldNotifyOnAllComponentOperations() {
		return notifyOnAllComponentOperations;
	}
	inline bool shouldNotifyOnAllEntityOperations() {
		return notifyOnAllEntityOperations;
	}

protected:
	void setNotificationSettings(
			bool shouldNotifyOnAllComponentOperations,
			bool shouldNotifyOnAllEntityOperations) {
		notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
		notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
	}
	void addComponentID(int id) {
		componentIDs.push_back(id);
	}
private:
	std::vector<int> componentIDs;
	bool notifyOnAllComponentOperations = false;
	bool notifyOnAllEntityOperations = false;
};

class ECS
{
public:
	ECS() {}
	~ECS();

	// ECSListener methods
	inline void addListener(ECSListener* listener) {
		listeners.push_back(listener);
	}

	// Entity methods
	EntityHandle makeEntity(BaseECSComponent** components, const int* componentIDs, size_t numComponents);
	void removeEntity(EntityHandle handle);

	template<class A>
	EntityHandle makeEntity(A& c1)
	{
		BaseECSComponent* components[] = { &c1 };
		int componentIDs[] = {A::ID};
		return makeEntity(components, componentIDs, 1);
	}

	template<class A, class B>
	EntityHandle makeEntity(A& c1, B& c2)
	{
		BaseECSComponent* components[] = { &c1, &c2 };
		int componentIDs[] = {A::ID, B::ID};
		return makeEntity(components, componentIDs, 2);
	}

	template<class A, class B, class C>
	EntityHandle makeEntity(A& c1, B& c2, C& c3)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3 };
		int componentIDs[] = {A::ID, B::ID, C::ID};
		return makeEntity(components, componentIDs, 3);
	}

	template<class A, class B, class C, class D>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4 };
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID};
		return makeEntity(components, componentIDs, 4);
	}

	template<class A, class B, class C, class D, class E>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5 };
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID};
		return makeEntity(components, componentIDs, 5);
	}

	template<class A, class B, class C, class D, class E, class F>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6};
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID};
		return makeEntity(components, componentIDs, 6);
	}

	template<class A, class B, class C, class D, class E, class F, class G>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7};
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID};
		return makeEntity(components, componentIDs, 7);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8};
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID};
		return makeEntity(components, componentIDs, 8);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9 };
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID};
		return makeEntity(components, componentIDs, 9);
	}

	template<class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
	EntityHandle makeEntity(A& c1, B& c2, C& c3, D& c4, E& c5, F& c6, G& c7, H& c8, I& c9, J& c10)
	{
		BaseECSComponent* components[] = { &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10 };
		int componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID, J::ID };
		return makeEntity(components, componentIDs, 10);
	}



	// Component methods
	template<class Component>
	inline void addComponent(EntityHandle entity, Component* component)
	{
		addComponentInternal(entity, handleToEntity(entity), Component::ID, component);
		for(int i = 0; i < listeners.size(); i++) {
			const std::vector<int>& componentIDs = listeners[i]->getComponentIDs();
			if(listeners[i]->shouldNotifyOnAllComponentOperations()) {
				listeners[i]->onAddComponent(entity, Component::ID);
			} else {
				for(int j = 0; j < componentIDs.size(); j++) {
					if(componentIDs[j] == Component::ID) {
						listeners[i]->onAddComponent(entity, Component::ID);
						break;
					}
				}
			}
		}
	}

	template<class Component>
	bool removeComponent(EntityHandle entity)
	{
		for(int i = 0; i < listeners.size(); i++) {
			const std::vector<int>& componentIDs = listeners[i]->getComponentIDs();
			for(int j = 0; j < componentIDs.size(); j++) {
				if(listeners[i]->shouldNotifyOnAllComponentOperations()) {
					listeners[i]->onRemoveComponent(entity, Component::ID);
				} else {
					if(componentIDs[j] == Component::ID) {
						listeners[i]->onRemoveComponent(entity, Component::ID);
						break;
					}
				}
			}
		}
		return removeComponentInternal(entity, Component::ID);
	}

	template<class Component>
	Component* getComponent(EntityHandle entity)
	{
		return (Component*)getComponentInternal(handleToEntity(entity), components[Component::ID], Component::ID);
	}

	BaseECSComponent* getComponentByType(EntityHandle entity, int componentID)
	{
		return getComponentInternal(handleToEntity(entity), components[componentID], componentID);
	}

	// System methods
	void updateSystems(ECSSystemList& systems, float delta);
	
private:
	std::map<int, std::vector<char>> components;
	std::vector<std::pair<int, std::vector<std::pair<int, int> > >* > entities;
	std::vector<ECSListener*> listeners;

	inline std::pair<int, std::vector<std::pair<int, int> > >* handleToRawType(EntityHandle handle)
	{
		return (std::pair<int, std::vector<std::pair<int, int> > >*)handle;
	}

	inline int handleToEntityIndex(EntityHandle handle)
	{
		return handleToRawType(handle)->first;
	}

	inline std::vector<std::pair<int, int> >& handleToEntity(EntityHandle handle)
	{
		return handleToRawType(handle)->second;
	}

	void deleteComponent(int componentID, int index);
	bool removeComponentInternal(EntityHandle handle, int componentID);
	void addComponentInternal(EntityHandle handle, std::vector<std::pair<int, int> >& entity, int componentID, BaseECSComponent* component);
	BaseECSComponent* getComponentInternal(std::vector<std::pair<int, int> >& entityComponents, std::vector<char>& array, int componentID);

	void updateSystemWithMultipleComponents(int index, ECSSystemList& systems, float delta, const std::vector<int>& componentTypes,
			std::vector<BaseECSComponent*>& componentParam, std::vector<std::vector<char>*>& componentArrays);
	int findLeastCommonComponent(const std::vector<int>& componentTypes, const std::vector<int>& componentFlags);

	//NULL_COPY_AND_ASSIGN(ECS);
};
