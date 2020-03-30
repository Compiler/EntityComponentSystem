#pragma once

#include "Component.h"

#include <iostream>

namespace Vofog{

	class System{
	protected:
		std::unordered_map<size_t, std::shared_ptr<Component>> m_components;
	
	public:
		
		template<typename T>
		void addComponent(std::shared_ptr<T> type){
			//_ASSERT(m_components[type->getID()] != nullptr);
			m_components[type_id<T>()] = type;
		}

		virtual void update() = 0;




	};



}