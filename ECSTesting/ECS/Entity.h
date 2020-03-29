#pragma once

#include <iostream>

namespace Vofog{


	typedef size_t EntityID;
	class Entity{
	private:
		const EntityID _ID;
		static size_t _ENTITY_COUNT;

	public:
		Entity():_ID(_ENTITY_COUNT++){
			
		}


		EntityID getID()const { return _ID; }
		void _PRINT_DEBUG_() const noexcept{
			std::cout << "Entity #" << _ID << "\n";
		}
	};

	size_t Entity::_ENTITY_COUNT = 0;

}