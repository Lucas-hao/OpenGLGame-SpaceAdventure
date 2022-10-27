#pragma once

#include <array>
#include <unordered_map>

#include "Camera.h"
#include "ColorTexture.h"
#include "dependencies/GLFW/glfw3.h"
#include "gameobjects/GameObject.h"
#include "gameobjects/Rocks.h"
#include "Light.h"
#include "Skybox.h"
#include "gameobjects/EnemyCraft.h"
#include "gameobjects/Planet.h"
#include "gameobjects/Spacecraft.h"


#define SCR_WIDTH 1080
#define SCR_HEIGHT 720
#define OPEN_GL_VERSION "#version 430"

class Engine
{
public:
    static Engine& getEngine()
    {
        static Engine globalEngine;
        static bool bInitialized = false;
        if (!bInitialized)
        {
            bInitialized = true;
            globalEngine.initialize();
        }
        return globalEngine;
    }

private:
    // Object in this game
    Spacecraft spaceCraft;
    Planet planet;
    Rocks rocks;
    std::array<EnemyCraft, 3> enemyCrafts;
    // Shaders
    Shader defaultShader;
    Shader planetShader;
    Shader rockInstanceShader;
    Shader skyboxShader;
    // Camera
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 88.0f));
    // Light
    std::array<Light, 2> lights;
    // Skybox
    Skybox skybox;
    
    glm::mat4 projMatrix = glm::mat4(1.0f);
    GLFWwindow* globalWindow = nullptr;

    bool bWin = false;

    // time
    double lastFrameTime = 0.0;
    double deltaTime = 0.0;
    int fps = 0;

    // Interaction
    double lastMousePosX = 0.0;
    double lastMousePosY = 0.0;
    int initX = 0;
    int initY = 0;

private:
    Engine() = default;
    ~Engine() = default;

    void initialize();
    void calculateFps();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void frameResizeCallback(GLFWwindow* window, int width, int height);
    static void getOpenGLInfo();

public:
    /* get the global projection matrix */
    static glm::mat4 getProjMatrix() { return getEngine().projMatrix; }

    /* get the global frame interval time */
    static double getDeltaTime() { return getEngine().deltaTime; }

    /* get the global camera object */
    static Camera getCamera() { return getEngine().camera; }

    /* get the global window object */
    static GLFWwindow* getWindow() { return getEngine().globalWindow; }

    void render();
    void renderGui();
    void execute();
    void terminate();
    void tick();

    void cameraMovement(CameraMovement direction)
    {
        camera.cameraMovement(direction, deltaTime);
    }

    void cameraRotation(float xoffset, float yoffset)
    {
        camera.cameraRotate(xoffset, yoffset);
    }

    void cameraToggleMode()
    {
        CameraMode cameraMode = camera.getMode();
        if (cameraMode == CameraMode::MOVEMENT_FREE)
        {
            camera.setMode(CameraMode::ALL_FREE);
            glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (cameraMode == CameraMode::ALL_FREE)
        {
            camera.setMode(CameraMode::ATTACH);
        }
        else if (cameraMode == CameraMode::ATTACH)
        {
            camera.setMode(CameraMode::MOVEMENT_FREE);
            glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    void lightIntensityControl(float delta)
    {
        float intensity = lights[0].getIntensity() + delta;
        intensity = std::min(1.0f, intensity);
        intensity = std::max(0.0f, intensity);
        lights[0].setIntensity(intensity);
    }

    glm::vec3 getSpaceCraftLoc() const { return spaceCraft.getPosition(); }
    bool getWin() const { return bWin; }
    void setWin(bool win) { bWin = win; }
    

    void setInitX(int xx) { if (xx >= 0) initX = xx; }
    void setInitY(int yy) { if (yy >= 0) initY = yy; }
};
