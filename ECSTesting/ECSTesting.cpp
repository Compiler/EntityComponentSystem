#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include "ECS/System.h"
#include "ECS/Entity.h"
#include "ECS/EntityManager.h"
#include "ECS/Component.h"

#define __PRETTY_FUNCTION__ __FUNCSIG__
#define assert _ASSERT

typedef int GameState;





class MovementControlSystem : Vofog::System{
public:

    MovementControlSystem(std::shared_ptr<Vofog::TransformComponent> t, std::shared_ptr<Vofog::InputComponent> i){
        addComponent(t);
        addComponent(i);
    }

    void update() override {
        std::shared_ptr<Vofog::TransformComponent> transform = 
            std::dynamic_pointer_cast<Vofog::TransformComponent>(m_components[Vofog::type_id<Vofog::TransformComponent>()]);
        std::shared_ptr<Vofog::InputComponent> input = 
            std::dynamic_pointer_cast<Vofog::InputComponent>(m_components[Vofog::type_id<Vofog::InputComponent>()]);



        for(int i = 0; i < input->dataQueue.size(); i++){
            char dir = input->dataQueue[i];
            if(dir == 'd')transform->xPos += 1;
            if(dir == 'a')transform->xPos -= 1;
            if(dir == 'w')transform->yPos += 1;
            if(dir == 's')transform->yPos -= 1;
        }
        input->dataQueue.clear();
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
char sys_input;
void takeInput(){
    std::cin >> sys_input;
}









int main()
{

    
























    Vofog::EntityManager manager;
    Vofog::Entity player;
    manager.addEntity(player.getID());
    std::shared_ptr<Vofog::TransformComponent> transform = std::make_shared<Vofog::TransformComponent>();transform->xPos = 4; transform->yPos = 6;
    std::shared_ptr<Vofog::InputComponent> input = std::make_shared<Vofog::InputComponent>();
    std::shared_ptr<Vofog::RenderableComponent> render = std::make_shared<Vofog::RenderableComponent>();
    render->data = '$';
    //manager.attachComponent(player, transform);
    //manager.attachComponent(player, input);
    manager.attachComponent(player, transform, input, render);

    MovementControlSystem system(manager.getComponent<Vofog::TransformComponent>(player), manager.getComponent<Vofog::InputComponent>(player));

  
    init();
    while(state){
        drawClear();
        takeInput();
        manager.getComponent<Vofog::InputComponent>(player)->dataQueue.push_back(sys_input);
        system.update();
        std::vector<Vofog::EntityID> renderables = manager.getAssociatedEntities<Vofog::RenderableComponent, Vofog::TransformComponent>();
        for(int i = 0; i < renderables.size(); i++){
            map[(int)manager.getComponent<Vofog::TransformComponent>(renderables[i])->xPos][(int)manager.getComponent<Vofog::TransformComponent>(renderables[i])->yPos]
                = manager.getComponent<Vofog::RenderableComponent>(renderables[i])->data;
        }
    }

    
}

