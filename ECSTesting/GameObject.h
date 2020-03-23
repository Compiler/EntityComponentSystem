#pragma once
#include <iostream>
#include <vector>

namespace Vofog{

	struct ComponentData{


	};


	class GameObject{

	private:
	//	std::vector <std::unique_ptr<Vofog::Component>> _components;


	public:

		GameObject(){
		
		}

		void init(){
		
		
		}

		void update(){


		}


		void destroy(){
		
		
		}

		template<typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs){

			//T* c(new T(std::forward<TArgs>(mArgs)...));
		//	_components.emplace_back(std::make_unique<T>(std::forward<TArgs >(mArgs)...));
		//	return _components.at(getComponentTypeID<T>());

		}




	};

}