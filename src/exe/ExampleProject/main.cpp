// Testing
#include <iostream>

#include "src/util/Shader.h"
#include "src/util/Primitives.h"
#include "src/util/OrbitCamera.h"

#include "externals/gl3w/include/GL/gl3w.h"
#include "externals/glfw/include/GLFW/glfw3.h"
#include "externals/glm/glm/glm.hpp"

// Global variables
OrbitCamera camera(glm::vec3(0, 0, 0), 15.f, 30.f, 2.0f, 0.1f, 10.f);
GLboolean buttonPressed = GL_FALSE;
GLfloat cursorX, cursorY, prevCursorX, prevCursorY = 0;

// GLFW callback for cursor
static void cursorCallback(GLFWwindow* pWindow, GLdouble xpos, GLdouble ypos)
{
	cursorX = (GLfloat)xpos;
	cursorY = (GLfloat)ypos;
}

// GLFW callback for mouse buttons
static void buttonsCallback(GLFWwindow* pWindow, GLint button, GLint action, GLint mods)
{
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
    GLFWwindow* pWindow = glfwCreateWindow(width, height, "ExampleProject", NULL, NULL);
    glfwMakeContextCurrent(pWindow);
    gl3wInit();

	// Callbacks
	glfwSetCursorPosCallback(pWindow, cursorCallback);
    glfwSetMouseButtonCallback(pWindow, buttonsCallback);
	glfwSetScrollCallback(pWindow, scrollCallback);

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

	// Prepare camera
	camera.update();

	// Prepare projection
	glm::mat4 projection = glm::perspective(glm::radians(45.f), (GLfloat)width / (GLfloat)height, 0.01f, 10.f);

	// Prepare shader
	Shader shader("Simple.vert", "Simple.geom", "Simple.frag");
	shader.compile();
	shader.bind();
	shader.updateUniform("projMatrix", projection);

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
    while (!glfwWindowShouldClose(pWindow))
    {
        // Calculate delta time per frame
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Clearing of buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		// Update shader (should be bound)
		shader.updateUniform("viewMatrix", camera.getViewMatrix());

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)primitives::cube.size());

        // Swap front and back buffers and poll events
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

	// Delete VBO and VAO
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

    // Termination of program
    glfwTerminate();
    return 0;
}
