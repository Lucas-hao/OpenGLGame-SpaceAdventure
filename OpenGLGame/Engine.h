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
    GameObject spaceCraft;
    GameObject planet;
    Rocks rocks;
    std::array<GameObject, 3> enemyCrafts;
    // Textures
    ColorTexture earthNormalTexture;
    ColorTexture earthColorTexture;
    ColorTexture spaceCraftTexture;
    ColorTexture enemyTravelTexture;
    ColorTexture enemyAlertTexture;
    ColorTexture rockTexture;
    ColorTexture goldTexture;
    ColorTexture sapphireTexture;
    // Shaders
    Shader defaultShader;
    Shader planetShader;
    Shader rockInstanceShader;
    Shader skyboxShader;
    // Camera
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 88.0f));
    // Light
    Light light0;
    Light light1;
    // Skybox
    Skybox skybox;


    glm::mat4 projMatrix = glm::mat4(1.0f);
    GLFWwindow* globalWindow = nullptr;

    // time
    double lastFrameTime = 0.0;
    double deltaTime = 0.0;
    int fps = 0;

    // rotation speed
    float craftRotateSpeed = 20.0f;
    float planetRotateSpeed = 20.0f;
    float enemyMovementSpeed = 10.0f;

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

    void spacecraftMovement(GameObject::TranslationDirection direction)
    {
        spaceCraft.objectMovement(direction, deltaTime);
    }

    void cameraMovement(CameraMovement direction)
    {
        camera.cameraMovement(direction, deltaTime);
    }

    void spaceCraftRotation(GameObject::RotationDirection direction, float degree)
    {
        spaceCraft.rotation(direction, degree);
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
        float intensity = light0.getIntensity() + delta;
        intensity = std::min(1.0f, intensity);
        intensity = std::max(0.0f, intensity);
        light0.setIntensity(intensity);
    }

    glm::vec3 getSpaceCraftLoc() const { return spaceCraft.getPosition(); }
    ColorTexture getRockTexture() const { return rockTexture; }
    ColorTexture getGoldTexture() const { return goldTexture; }
    ColorTexture getSapphireTexture() const { return sapphireTexture; }

    void setInitX(int xx) { if (xx >= 0) initX = xx; }
    void setInitY(int yy) { if (yy >= 0) initY = yy; }
};
