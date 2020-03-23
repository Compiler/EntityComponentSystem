#include "pch.h"
#include <iostream>
#include "ECS/Component.h"
#include "ECS/GraphicsComponent.h"
#include "GameObject.h"
#include <string>

#define __PRETTY_FUNCTION__ __FUNCSIG__


typedef int GameState;


GameState state = 1;

void drawClear(){


    int hp = 100;
    for(int i = 0; i < 50; i++) std::cout << "\n";
    std::cout << "\t O        " << hp << "\n";
    std::cout << "\t/|\\ \n";
    std::cout << "\t/ \\ \n";
    std::cout << "-----------------------------------------\n";

}
void takeInput(){
    char input;
    std::cin >> input;

}

std::string test(int a, int b){
    return std::to_string(a) + std::to_string(b);
}
int main()
{


    Vofog::PositionComponent comp;
    std::cout << comp.ID;
 
    std::cin.get();

    std::exit(1);
    while(state){
        drawClear();
        takeInput();
    }

    
}

