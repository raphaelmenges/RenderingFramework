// Testing
#include <iostream>

#include "src/util/Shader.h"
#include "src/util/Primitives.h"
#include "src/util/OrbitCamera.h"

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

	// Perpare camera
	OrbitCamera camera(glm::vec3(0,0,0), 15.f, 30.f, 0.5f, 0.1f, 10.f);
	camera.update();

	// Prepare shader
	Shader shader("Simple.vert", "Simple.frag");
	shader.compile();
	shader.bind();
	shader.updateUniform("viewMatrix", camera.getViewMatrix());

	// Prepare mesh
	GLuint VBO = 0; // handle of VBO
	glGenBuffers(1, &VBO); // generate VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // set as current VBO
	glBufferData(GL_ARRAY_BUFFER, primitives::cube.size() * sizeof(GLfloat), primitives::cube.data(), GL_STATIC_DRAW); // copy data to GPU

	// Prepare vertex array object
	GLuint VAO = 0; // handle of VAO
	glGenVertexArrays(1, &VAO); // generate VAO
	glBindVertexArray(VAO); // set as current VAO

	// Add vertices to VAO
	GLuint vertexAttrib = glGetAttribLocation(shader.getProgram(), "position");
	glEnableVertexAttribArray(vertexAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time per frame
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Clearing of buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)primitives::cube.size());

        // Swap front and back buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// Delete VBO and VAO
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

    // Termination of program
    glfwTerminate();
    return 0;
}
