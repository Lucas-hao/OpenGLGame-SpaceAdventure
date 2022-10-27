#include "Engine.h"

#include "dependencies/GLFW/glfw3.h"
#include "dependencies/imgui/imgui_impl_glfw.h"
#include "dependencies/imgui/imgui_impl_opengl3.h"

/*
 * Initialize the engine including:
 * 1. the data for rendering (object, shader, texture, camera, parameters)
 * 2. the glfw window
 * 3. the imgui initialization
 */
void Engine::initialize()
{
    /* initialize the glfw */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    /* glfw: configure; necessary for MAC */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    globalWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Spacecraft Adventure", nullptr, nullptr);
    if (!globalWindow)
    {
        std::cout << "Failed to create GLFW globalWindow" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    /* Make the globalWindow's context current */
    glfwMakeContextCurrent(globalWindow);

    /* by default, free camera mode */
    glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW not OK." << std::endl;
    }

    getOpenGLInfo();
    glfwSetKeyCallback(globalWindow, keyCallback);
    glfwSetCursorPosCallback(globalWindow, cursorPositionCallback);
    glfwSetFramebufferSizeCallback(globalWindow, frameResizeCallback);

    // Light setup
    lights[0] = Light(glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(0.9f, 0.72f, 0.0f));
    lights[1] = Light(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(1.0f, 0.5f, 0.0f));

    // ProjMatrix setup
    projMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / SCR_HEIGHT, 0.1f, 500.0f);

    // Default game object initialization
    // Rocks rocks;
    spaceCraft.initialize("SpaceCraft", "resources/object/spacecraft.obj");
    planet.initialize("Planet", "resources/object/planet.obj");
    rocks.initialize("Rocks", "resources/object/rock.obj");
    enemyCrafts[0].initialize("EnemyCraft0", "resources/object/craft.obj", glm::vec3(-30.0f, 0.0f, 30.0f));
    enemyCrafts[1].initialize("EnemyCraft1", "resources/object/craft.obj", glm::vec3(0.0f, 0.0f, 40.0f));
    enemyCrafts[2].initialize("EnemyCraft2", "resources/object/craft.obj", glm::vec3(30.0f, 0.0f, 50.0f));

    // shader set up
    defaultShader.setupShader("resources/shader/VertexShaderCode.glsl", "resources/shader/FragmentShaderCode.glsl");
    planetShader.setupShader("resources/shader/planetv.glsl", "resources/shader/planetf.glsl");
    rockInstanceShader.setupShader("resources/shader/InstanceVertexShader.glsl",
                                   "resources/shader/InstanceFragmentShader.glsl");
    skyboxShader.setupShader("resources/shader/SkyboxVertex.glsl", "resources/shader/SkyboxFragment.glsl");

    planetShader.use();
    planetShader.setInt("diffuseMap", 0);
    planetShader.setInt("normalMap", 1);

    rockInstanceShader.use();
    rockInstanceShader.setInt("objTexture", 0);

    // Skybox init
    skybox.Initialize();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);

    // Initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(globalWindow, true);
    ImGui_ImplOpenGL3_Init(OPEN_GL_VERSION);
}

void Engine::render()
{
    glm::mat4 viewMatrix = camera.getViewMatrix();

    defaultShader.use();
    defaultShader.setMat4("proj", projMatrix);
    defaultShader.setMat4("view", viewMatrix);
    // Point Light 1
    defaultShader.setVec3("pointLights[0].position", lights[0].getPosition());
    defaultShader.setVec3("pointLights[0].color", lights[0].getFinalColor());
    defaultShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
    defaultShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    defaultShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    defaultShader.setFloat("pointLights[0].constant", 1.0f);
    defaultShader.setFloat("pointLights[0].linear", 0.00009f);
    defaultShader.setFloat("pointLights[0].quadratic", 0.000032f);
    // Point Light 2
    defaultShader.setVec3("pointLights[1].position", lights[1].getPosition());
    defaultShader.setVec3("pointLights[1].color", lights[1].getFinalColor());
    defaultShader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
    defaultShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
    defaultShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    defaultShader.setFloat("pointLights[1].constant", 1.0f);
    defaultShader.setFloat("pointLights[1].linear", 0.00009f);
    defaultShader.setFloat("pointLights[1].quadratic", 0.000032f);
    // View position
    defaultShader.setVec3("viewPos", camera.getPosition());
    spaceCraft.draw(defaultShader);
    rocks.drawSpecialRocks(defaultShader);
    enemyCrafts[0].draw(defaultShader);
    enemyCrafts[1].draw(defaultShader);
    enemyCrafts[2].draw(defaultShader);

    planetShader.use();
    planetShader.setMat4("proj", projMatrix);
    planetShader.setMat4("view", viewMatrix);
    planetShader.setVec3("lightPos[0]", lights[0].getPosition());
    planetShader.setVec3("lightPos[1]", lights[1].getPosition());
    planetShader.setVec3("viewPos", camera.getPosition());
    // Point Light 0
    planetShader.setVec3("pointLights[0].color", lights[0].getFinalColor());
    planetShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
    planetShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    planetShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    planetShader.setFloat("pointLights[0].constant", 1.0f);
    planetShader.setFloat("pointLights[0].linear", 0.00009f);
    planetShader.setFloat("pointLights[0].quadratic", 0.000032f);
    // Point Light 1
    planetShader.setVec3("pointLights[1].color", lights[1].getFinalColor());
    planetShader.setVec3("pointLights[1].ambient", 0.3f, 0.3f, 0.3f);
    planetShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f);
    planetShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    planetShader.setFloat("pointLights[1].constant", 1.0f);
    planetShader.setFloat("pointLights[1].linear", 0.00009f);
    planetShader.setFloat("pointLights[1].quadratic", 0.000032f);
    planet.draw(planetShader);


    // draw the rocks
    rockInstanceShader.use();
    rockInstanceShader.setMat4("proj", projMatrix);
    rockInstanceShader.setMat4("view", viewMatrix);
    // Point Light 1
    rockInstanceShader.setVec3("pointLights[0].position", lights[0].getPosition());
    rockInstanceShader.setVec3("pointLights[0].color", lights[0].getFinalColor());
    rockInstanceShader.setVec3("pointLights[0].ambient", 0.4f, 0.4f, 0.4f);
    rockInstanceShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
    rockInstanceShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    rockInstanceShader.setFloat("pointLights[0].constant", 1.0f);
    rockInstanceShader.setFloat("pointLights[0].linear", 0.0009f);
    rockInstanceShader.setFloat("pointLights[0].quadratic", 0.00032f);
    // Point Light 2
    rockInstanceShader.setVec3("pointLights[1].position", lights[1].getPosition());
    rockInstanceShader.setVec3("pointLights[1].color", lights[1].getFinalColor());
    rockInstanceShader.setVec3("pointLights[1].ambient", 0.4f, 0.4f, 0.4f);
    rockInstanceShader.setVec3("pointLights[1].diffuse", 0.3f, 0.3f, 0.3f);
    rockInstanceShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    rockInstanceShader.setFloat("pointLights[1].constant", 1.0f);
    rockInstanceShader.setFloat("pointLights[1].linear", 0.0009f);
    rockInstanceShader.setFloat("pointLights[1].quadratic", 0.00032f);
    // View position
    rockInstanceShader.setVec3("viewPos", camera.getPosition());
    rocks.drawNormalRocks(rockInstanceShader);

    skybox.draw(skyboxShader);
}

void Engine::renderGui()
{
    static bool show_demo_window = true;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // call render for all the object needed to be rendered in engine
    ImGui::Begin("Engine GUI");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::Text("glfw application FPS: %d", fps);
    ImGui::Text("Another calculated FPS: %.2f", 1 / deltaTime);

    switch (camera.getMode())
    {
    case CameraMode::ATTACH:
        ImGui::Text("Attach game mode");
        break;
    case CameraMode::MOVEMENT_FREE:
        ImGui::Text("Movement free camera mode");
        break;
    case CameraMode::ALL_FREE:
        ImGui::Text("All free camera mode");
        break;
    }
    camera.guiRender();
    spaceCraft.guiRender();
    for (size_t i = 0; i < enemyCrafts.size(); ++i)
    {
        enemyCrafts[i].guiRender();
    }
    planet.guiRender();
    rocks.guiRender();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Engine::execute()
{
    while (!glfwWindowShouldClose(globalWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
        glViewport(initX, initY, SCR_WIDTH, SCR_HEIGHT);

        render();
        renderGui();

        glfwSwapBuffers(globalWindow);
        glfwPollEvents();
        tick();
    }
    terminate();
}

void Engine::terminate()
{
    glfwDestroyWindow(globalWindow);
    glfwTerminate();
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::calculateFps()
{
    static int numOfFrames = 0;
    static double lastSecTime = 0.0;
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    numOfFrames += 1;
    if (currentTime - lastSecTime >= 1)
    {
        fps = numOfFrames;
        numOfFrames = 0;
        lastSecTime = currentTime;
    }
    lastFrameTime = glfwGetTime();
}


void Engine::tick()
{
    // A rough calculation of fps
    calculateFps();

    if (camera.getMode() == CameraMode::ATTACH)
    {
        camera.updateCameraVectors(
            spaceCraft.getPosition() - spaceCraft.getZDirection() * glm::vec3(8.0f) + glm::vec3(0.0f, 4.0f, 0.0f),
            spaceCraft.getPosition() + spaceCraft.getZDirection() * glm::vec3(10.0f));
    }

    planet.tick();
    rocks.tick();
    for (size_t i = 0; i < enemyCrafts.size(); ++i) enemyCrafts[i].tick();
}

void Engine::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Engine& engine = getEngine();
    static bool firstMouseMovement = true;
    if (firstMouseMovement)
    {
        firstMouseMovement = false;
        engine.lastMousePosX = static_cast<float>(xpos);
        engine.lastMousePosY = static_cast<float>(ypos);
    }

    double xoffset = xpos - engine.lastMousePosX;
    double yoffset = engine.lastMousePosY - ypos;
    // reversed since y-coordinates go from bottom to top

    float deltaX = static_cast<float>(engine.lastMousePosX - xpos);
    if (engine.camera.getMode() == CameraMode::ATTACH)
    {
        engine.spaceCraft.spacecraftRotation(deltaX);
    }
    else if (engine.camera.getMode() == CameraMode::ALL_FREE)
    {
        engine.cameraRotation(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
    engine.lastMousePosX = xpos;
    engine.lastMousePosY = ypos;
}

void Engine::frameResizeCallback(GLFWwindow* window, int width, int height)
{
    Engine& engine = getEngine();
    engine.setInitX((width - SCR_WIDTH) / 2);
    engine.setInitY((height - SCR_HEIGHT) / 2);
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine& engine = getEngine();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    switch (engine.camera.getMode())
    {
    case CameraMode::ATTACH:
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            engine.spaceCraft.spacecraftMovement(TranslationDirection::FRONT, static_cast<float>(engine.deltaTime));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            engine.spaceCraft.spacecraftMovement(TranslationDirection::BACK, static_cast<float>(engine.deltaTime));
            // engine.spacecraftMovement(TranslationDirection::BACK);
        }
    // Notice that our view point are from the camera
    // so the right and left translation are reversed
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            engine.spaceCraft.spacecraftMovement(TranslationDirection::RIGHT, static_cast<float>(engine.deltaTime));
            // engine.spacecraftMovement(TranslationDirection::RIGHT);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            engine.spaceCraft.spacecraftMovement(TranslationDirection::LEFT, static_cast<float>(engine.deltaTime));
            // engine.spacecraftMovement(TranslationDirection::LEFT);
        }
        break;
    case CameraMode::MOVEMENT_FREE:
    case CameraMode::ALL_FREE:
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            engine.cameraMovement(CameraMovement::FORWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            engine.cameraMovement(CameraMovement::BACKWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            engine.cameraMovement(CameraMovement::LEFT);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            engine.cameraMovement(CameraMovement::RIGHT);
        }
        break;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) { engine.lightIntensityControl(0.1f); }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) { engine.lightIntensityControl(-0.1f); }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) { engine.cameraToggleMode(); }
}

void Engine::getOpenGLInfo()
{
    // OpenGL information
    const GLubyte* name = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    std::cout << "OpenGL company: " << name << std::endl;
    std::cout << "Renderer name: " << renderer << std::endl;
    std::cout << "OpenGL version: " << glversion << std::endl;
}
