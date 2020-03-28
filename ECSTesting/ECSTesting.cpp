#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#define __PRETTY_FUNCTION__ __FUNCSIG__
#define assert _ASSERT

typedef int GameState;


struct SpriteComponent{
    char data;
};

struct TransformComponent{
    float xPos, yPos;
};

struct InputComponent{
    std::vector<char> dataQueue;
};


class MovementControlSystem{
public:

    MovementControlSystem(){
       // addComponentType(TransformComponent::ID);
       // addComponentType(InputComponent::ID);
    }

    void updateComponents(void** components){
        TransformComponent* transform = (TransformComponent*)components[0];
        InputComponent* input = (InputComponent*)components[1];

        for(int i = 0; i < input->dataQueue.size(); i++){
            char dir = input->dataQueue[i];
            if(dir == 'd')transform->xPos += 1;
            if(dir == 'a')transform->xPos -= 1;
            if(dir == 'w')transform->yPos += 1;
            if(dir == 's')transform->yPos -= 1;
        }
        //input->dataQueue.clear();
    }


};
GameState state = 1;
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
char map[MAP_WIDTH][MAP_HEIGHT];

void init(){
    for(int y = MAP_HEIGHT; y > 0; y--){
        for(int x = 0; x < MAP_WIDTH; x++){
            map[x][y] = '_';
        }
    }
}

void drawClear(){

    for(int i = 0; i < 50; i++)std::cout << "\n";
    for(int y = MAP_HEIGHT; y > 0; y--){
        for(int x = 0; x < MAP_WIDTH; x++){
            std::cout << map[x][y] << "|";
        }
        std::cout << "\n";
    }

}
void takeInput(){
    char input;
    std::cin >> input;

}

std::string test(int a, int b){
    return std::to_string(a) + std::to_string(b);
}


typedef void* IntHandle;


void printData(IntHandle handle){
    std::cout << (int)(handle)<< "\n";
}

class Functor{

public:

    int a;
    int b;
    void operator()() {
        std::cout << a << " + " << b << " = " << a + b << "\n";
    }

};

void test_function(int& a){
    std::cout << a << "\n";

}
int main()
{

    int a = 4;
    std::thread my_thread(test_function, std::ref(a));
    my_thread.join();
    assert(!my_thread.joinable());
    exit(std::cin.get());



























    TransformComponent transformComponent;
    transformComponent.xPos = 5;
    transformComponent.yPos = 7;
    SpriteComponent spriteComponent;
    spriteComponent.data = '&';

    InputComponent inputComponent;


    MovementControlSystem system;

    init();
    while(state){
        drawClear();
        takeInput();
    }

    
}

