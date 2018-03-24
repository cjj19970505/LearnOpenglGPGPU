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
string loadFile(string filePath);
void computeTaskInit();
void computeTaskLoop();

GLuint texBuffer;
GLuint tex;
GLuint program = 0;
const int PARTICLE_COUNT = 100;
const int PARTICLE_GROUP_COUNT = 10;
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
	computeTaskInit();
	//computeTaskLoop();
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
		glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		testShader.use();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_TEXTURE_BUFFER, texBuffer);
		glm::vec4 *positions = (glm::vec4*)glMapBufferRange(GL_TEXTURE_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			//ostringstream iss;
			//iss << "(" << positions[i].x << ", " << positions[i].y << ", " << positions[i].z << ", " << positions[i].w << ")";
			positions[i];
			//cout << iss.str() << "\n";
		}
		glUnmapBuffer(GL_TEXTURE_BUFFER);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

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



void computeTaskInit() 
{
	//===Init Compute Data
	
	
	//GLuint positionBuffer;
	
	glGenBuffers(1, &texBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, texBuffer);
	glBufferData(GL_TEXTURE_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
	glm::vec4 *positions = (glm::vec4*)glMapBufferRange(GL_TEXTURE_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = glm::vec4(1, 0, 1, 1);
	}
	glUnmapBuffer(GL_TEXTURE_BUFFER);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_BUFFER, tex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, texBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	string cShaderCode = loadFile("TestComputeShader.glsl");
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	const GLchar *source[]{ cShaderCode.c_str() };
	glShaderSource(shader, 1, source, NULL);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	char infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	program = shader;
	

}
void computeTaskLoop()
{
	glUseProgram(program);
	glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindBuffer(GL_TEXTURE_BUFFER, texBuffer);
	glm::vec4 *positions = (glm::vec4*)glMapBufferRange(GL_TEXTURE_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		ostringstream iss;
		iss << "(" << positions[i].x << ", " << positions[i].y << ", " << positions[i].z << ", " << positions[i].w << ")";
		cout << iss.str() << "\n";
	}
	glUnmapBuffer(GL_TEXTURE_BUFFER);


}
string loadFile(string filePath)
{
	std::string vertexCode;
	std::ifstream vShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(filePath);
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		vertexCode = vShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return vertexCode;
}

