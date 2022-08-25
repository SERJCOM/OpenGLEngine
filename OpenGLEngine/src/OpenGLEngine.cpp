﻿#include "init.h"
#include <reactphysics3d/utils/DebugRenderer.h>
#include "physDebug.hpp"

int main() {

    world->setIsDebugRenderingEnabled(true);
    DebugRenderer& debugRenderer = world->getDebugRenderer();
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
    std::cout << debugRenderer.getNbLines() << " " << debugRenderer.getNbTriangles() <<  std::endl;

    Window window(800, 600);
    Engine engine;
    Camera camera(&window.window);
    Shader shad("D:/prog/проекты VISUAL STUDIO/OpenGLEngine/OpenGLEngine/shaders/shader.vert", "D:/prog/проекты VISUAL STUDIO/OpenGLEngine/OpenGLEngine/shaders/shader.frag");

    
                       
    Model obj("D:/prog/obj/mine/Grass_Block.obj", world, &physicsCommon);
    obj.CreatePhysicsBody();
    obj.CreateCollisionBox(glm::vec3(10.0f, 5.0f, 10.0f));
    obj.SetTypeOfThePhysObject(false);
    //obj.RotateObject(glm::vec3(180.0f, 180.0f, 0));
    obj.MoveObject(0, 1000, 0);
    

    /*Model city("D:/prog/obj/dimaMap/untitled.obj", world, &physicsCommon);
    city.CreatePhysicsBody();
    city.CreateCollisionBox(glm::vec3(50.0f, 5.0f, 50.0f));
    city.SetTypeOfThePhysObject(true);*/

    

    const decimal timeStep = 1.0f / 60.0f;
    float i = 0;
    while (window.running) {
        engine.ClearBuffers();
        camera.move();
        camera.looking(&window.window);
        camera.view = camera.updateView();
        //cout << camera.cameraPos.x << " " << camera.cameraPos.y << " " << camera.cameraPos.z << endl;
        world->update(timeStep);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
        glm::mat4 view = camera.view;
        shad.use();
        shad.setMat4("projection", projection);
        shad.setMat4("view", view);
        shad.setVec3("lightPos", glm::vec3(0.0f, 50.0f, 0.0f));


        //obj.PrintObjectPosition();
        //obj.RotateObject(glm::vec3(18.0f, 0, 0));
        obj.UpdateObjectTransform();
        obj.Draw(shad);

        /*city.UpdateObjectTransform();
        city.Draw(shad);*/

        

        window.Display();
        i += 0.01f;
    }
    return 0;
}