#include <iostream>

#include "externals/gl3w/include/GL/gl3w.h"
#include "externals/glfw/include/GLFW/glfw3.h"
#include "externals/glm/glm/glm.hpp"

int main()
{
    // Window and OpenGL initialization
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ExampleProject", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Prepare delta time calculation
    float lastTime, deltaTime;
    lastTime = (float)glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time per frame
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Test
        glm::vec4 clearColor(1,1,0,1);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

        // Clearing of buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Swap front and back buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Termination of program
    glfwTerminate();
    return 0;
}
