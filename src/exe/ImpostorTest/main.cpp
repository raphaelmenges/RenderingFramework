// Testing
#include <iostream>

#include "src/util/ShaderProgram.h"
#include "src/util/Primitives.h"
#include "src/util/OrbitCamera.h"

#include "externals/gl3w/include/GL/gl3w.h"
#include "externals/glfw/include/GLFW/glfw3.h"
#include "externals/glm/glm/glm.hpp"
#include "externals/imgui/imgui.h"
#include "externals/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h"

// #########################################
// ##############SHORTCUTS##################
// #########################################
// C = Toggle RENDER_SPHERE
// I = Toggle TRIANGLE_IMPOSTOR
// #########################################

// Definitions
const int resolution = 12;
const std::string RENDER_SPHERE = "RENDER_SPHERE";
const std::string TRIANGLE_IMPOSTOR = "TRIANGLE_IMPOSTOR";

// Global variables
OrbitCamera camera(glm::vec3(0, 0, 0), 90.f, 90.f, 5.0f, 1.0f, 20.0f);
GLboolean buttonPressed = GL_FALSE;
GLfloat cursorX, cursorY, prevCursorX, prevCursorY = 0;
ShaderProgram shaderProgram("ImpostorTest/Impostor.vert", "ImpostorTest/Impostor.geom", "ImpostorTest/Impostor.frag");

// GLFW callback for cursor
static void cursorCallback(GLFWwindow* pWindow, GLdouble xpos, GLdouble ypos)
{
    // Check whether ImGui is handling this
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse)
    {
        return;
    }

    cursorX = (GLfloat)xpos;
    cursorY = (GLfloat)ypos;
}

// GLFW callback for mouse buttons
static void buttonsCallback(GLFWwindow* pWindow, GLint button, GLint action, GLint mods)
{
    // Check whether ImGui is handling this
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureMouse)
    {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (action == GLFW_PRESS)
        {
            buttonPressed = GL_TRUE;
        }
        else if (action == GLFW_RELEASE)
        {
            buttonPressed = GL_FALSE;
        }
    }
}

// GLFW callback for mouse scrolling
static void scrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset)
{
    camera.setRadius(camera.getRadius() - 0.1f * (float)yoffset);
}

// GLFW callback for keyboard
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check whether ImGui is handling this
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureKeyboard)
    {
        return;
    }

    bool recompileShaderProgram = false;

    // Display primitive itself or sphere
    if(key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if(shaderProgram.findDefine(RENDER_SPHERE))
        {
            shaderProgram.removeDefine(RENDER_SPHERE);
        }
        else
        {
            shaderProgram.addDefine(RENDER_SPHERE);
        }
        recompileShaderProgram = true;
    }

    // Change impostor geometry
    if(key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        if(shaderProgram.findDefine(TRIANGLE_IMPOSTOR))
        {
            shaderProgram.removeDefine(TRIANGLE_IMPOSTOR);
        }
        else
        {
            shaderProgram.addDefine(TRIANGLE_IMPOSTOR);
        }
        recompileShaderProgram = true;
    }

    // Recompile shader program if necessary
    if(recompileShaderProgram)
    {
        shaderProgram.compile();
    }
}

int main()
{
    // Window and OpenGL initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLuint width = 1280;
    GLuint height = 720;
    GLFWwindow* pWindow = glfwCreateWindow(width, height, "ImpostorTest", NULL, NULL);
    glfwMakeContextCurrent(pWindow);
    gl3wInit();

    // Init ImGui
    ImGui_ImplGlfwGL3_Init(pWindow, true);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Set callbacks after ImGui
    glfwSetCursorPosCallback(pWindow, cursorCallback);
    glfwSetMouseButtonCallback(pWindow, buttonsCallback);
    glfwSetScrollCallback(pWindow, scrollCallback);
    glfwSetKeyCallback(pWindow, keyCallback);

    // Prepare OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // Prepare delta time calculation
    float lastTime, deltaTime;
    lastTime = (float)glfwGetTime();

    // Clear color
    glm::vec4 clearColor(1, 1, 0, 1);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

    // Prepare projection
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (GLfloat)width / (GLfloat)height, 0.01f, 100.f);

    // Prepare shader program
    shaderProgram.addDefine(RENDER_SPHERE);
    shaderProgram.compile();

    // Prepare vertex array object (even if empty, it seams necessary)
    GLuint VAO = 0; // handle of VAO
    glGenVertexArrays(1, &VAO); // generate VAO
    glBindVertexArray(VAO); // set as current VAO

    // Main loop
    while (!glfwWindowShouldClose(pWindow))
    {
        // Calculate delta time per frame
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Clearing of buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui new frame
        ImGui_ImplGlfwGL3_NewFrame();

        // Calculate cursor movement
        GLfloat cursorDeltaX = cursorX - prevCursorX;
        GLfloat cursorDeltaY = cursorY - prevCursorY;
        prevCursorX = cursorX;
        prevCursorY = cursorY;

        // Update camera
        if (buttonPressed)
        {
            camera.setAlpha(camera.getAlpha() + 0.25f * cursorDeltaX);
            camera.setBeta(camera.getBeta() - 0.25f * cursorDeltaY);
        }
        camera.update();

        // Update shader program (should be bound)
        shaderProgram.updateUniform("viewMatrix", camera.getViewMatrix());
        shaderProgram.updateUniform("cameraWorldPos", camera.getPosition());

        // Those values are static but must be updated each frame because a
        // recompilation of the shader program leads to loss of uniform data (BUG)
        shaderProgram.updateUniform("resolution", resolution);
        shaderProgram.updateUniform("projMatrix", projection);

        // Draw cube
        glDrawArrays(GL_POINTS, 0, (GLsizei)glm::pow(resolution,3));

        // ImGui
        bool opened = true;
        ImGui::Begin("Properties", &opened, ImVec2(300, 100));
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();

        // Swap front and back buffers and poll events
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    // Delete VAO
    glDeleteVertexArrays(1, &VAO);

    // Termination of program
    glfwTerminate();
    return 0;
}
