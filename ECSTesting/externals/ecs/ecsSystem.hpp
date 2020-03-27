#pragma once

#include "ecsComponent.hpp"

class BaseECSSystem
{
public:
	enum
	{
		FLAG_OPTIONAL = 1,
	};
	BaseECSSystem() {}
	~BaseECSSystem() {}
	virtual void updateComponents(float delta, BaseECSComponent** components) {}
	const std::vector<int>& getComponentTypes()
	{
		return componentTypes;
	}
	const std::vector<int>& getComponentFlags()
	{
		return componentFlags;
	}
	bool isValid();
protected:
	void addComponentType(int componentType, int componentFlag = 0)
	{
		componentTypes.push_back(componentType);
		componentFlags.push_back(componentFlag);
	}
private:
	std::vector<int> componentTypes;
	std::vector<int> componentFlags;
};

class ECSSystemList
{
public:
	inline bool addSystem(BaseECSSystem& system)
	{
		if(!system.isValid()) {
			return false;
		}
		systems.push_back(&system);
		return true;
	}
	inline size_t size() {
		return systems.size();
	}
	inline BaseECSSystem* operator[](int index) {
		return systems[index];
	}
	bool removeSystem(BaseECSSystem& system);
private:
	std::vector<BaseECSSystem*> systems;
};
