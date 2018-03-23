// LearnOpenglGPGPU.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Camera.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


using namespace std;
using namespace xjg;

int screenWidth = 1024;
int screenHeight = 1024;
Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, -90, 0));


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int loadTestVAO();
glm::mat4 getProjMat();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "LearnGPGPU", nullptr, nullptr);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int testVAO = loadTestVAO();
	Shader testShader("TestVertexShader.glsl", "TestFragmentShader.glsl");
	testShader.loadShader();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
		model = glm::translate(model, glm::vec3(0,0,0));

		glBindVertexArray(testVAO);
		testShader.setMatrix4("app.model", model);
		testShader.setMatrix4("app.view", camera.getViewMatrix());
		testShader.setMatrix4("app.projection", getProjMat());
		testShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Use to check if the camera is working properly
int loadTestVAO() 
{
	float testVert[]{ -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f };
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testVert), testVert, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	return VAO;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	camera.processInput(window);
}

glm::mat4 getProjMat()
{
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.fov), (float)(screenWidth / screenHeight), 0.1f, 100.0f);
	return projection;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

