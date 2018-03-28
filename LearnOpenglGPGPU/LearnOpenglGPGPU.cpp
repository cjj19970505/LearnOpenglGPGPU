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
#include <stb_image.h>


using namespace std;
using namespace xjg;

int screenWidth = 1024;
int screenHeight = 1024;
Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, -90, 0));


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

GLuint loadScreenVAO();
int loadTestVAO();
glm::mat4 getProjMat();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
string loadFile(string filePath);
void loadTestPic(unsigned char *data, int width, int height);

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

	GLuint screenVAO = loadScreenVAO();
	Shader screenShader("ScreenVertShader.glsl", "ScreenFragShader.glsl");
	screenShader.loadShader();
	//==========================


	//===========Init Compute Shader=============
	string cShaderCode1 = loadFile("TestComputeShader.glsl");
	GLuint cShader1 = glCreateShader(GL_COMPUTE_SHADER);
	const char *cCodeChar1 = cShaderCode1.c_str();
	glShaderSource(cShader1, 1, &cCodeChar1, NULL);
	glCompileShader(cShader1);
	int success;
	char infoLog[512];
	glGetShaderiv(cShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(cShader1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint computeProg1 = glCreateProgram();
	glAttachShader(computeProg1, cShader1);
	glLinkProgram(computeProg1);
	glGetProgramiv(computeProg1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(computeProg1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(cShader1);
	
	int width, height, nrChannels;
	unsigned char *data = stbi_load("pic.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		cout << "LoadPic::(" << width << ", " << height << ", " << nrChannels << ")" << endl;
		//stbi_image_free(data);
	}
	else
	{
		cout << "ERROR::LOADING PIC ERROR" << endl;
	}
	
	//const int width = 256;
	//const int height = 256;
	//unsigned char data[width*height * 4];
	//loadTestPic(data, width, height);

	GLuint inputImage;
	glGenTextures(1, &inputImage);
	glBindTexture(GL_TEXTURE_2D, inputImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	GLuint outputImage;
	glGenTextures(1, &outputImage);
	glBindTexture(GL_TEXTURE_2D, outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 1024, 1024);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint outputArray;
	glGenTextures(1, &outputArray);
	glBindTexture(GL_TEXTURE_1D, outputArray);
	glTexStorage1D(GL_TEXTURE_1D, 8, GL_RGBA32F, 1024);


	//==========================
	glClearColor(1, 0, 0, 1);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glUseProgram(computeProg1);
		for (int i = 0; i < 1; i++) 
		{
			glBindImageTexture(0, inputImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
			glBindImageTexture(1, outputImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			glBindImageTexture(2, outputArray, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			glDispatchCompute(1, 1024, 1);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputImage);
		screenShader.use();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputImage);
		
		glBindVertexArray(screenVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
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
void loadTestPic(unsigned char *data, int width, int height)
{
	const int picWidth = width;
	const int picHeight = height;
	for (int x = 0; x < picWidth; ++x)
	{
		for (int y = 0; y < picHeight; ++y)
		{
			int i = y * picHeight + x;
			if (x < picWidth / 2 && y < picWidth / 2)
			{
				data[i * 4] = 255;
				data[i * 4 + 1] = 0;
				data[i * 4 + 2] = 0;
				data[i * 4 + 3] = 255;
			}
			if (x >= picWidth / 2 && y < picWidth / 2)
			{
				data[i * 4] = 0;
				data[i * 4 + 1] = 255;
				data[i * 4 + 2] = 0;
				data[i * 4 + 3] = 255;
			}
			if (x >= picWidth / 2 && y >= picWidth / 2)
			{
				data[i * 4] = 0;
				data[i * 4 + 1] = 0;
				data[i * 4 + 2] = 255;
				data[i * 4 + 3] = 255;
			}
			else
			{
				data[i * 4] = 0;
				data[i * 4 + 1] = 255;
				data[i * 4 + 2] = 255;
				data[i * 4 + 3] = 255;
			}
		}
	}
	
}
GLuint loadScreenVAO()
{
	GLfloat deviceVert[]{
		-1, -1, 0, 0,	//前两个是设备坐标 后两个是texCoord
		1, -1, 1, 0,
		-1, 1, 0, 1,
		1, 1, 1, 1
	};
	GLuint indices[]{
		0, 1, 2, 2, 1, 3
	};
	
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(deviceVert), deviceVert, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
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
	GLuint texBuffer;
	GLuint tex;
	GLuint program = 0;
	const int PARTICLE_COUNT = 100;
	const int PARTICLE_GROUP_COUNT = 10;
	
	//GLuint positionBuffer;
	
	glGenBuffers(1, &texBuffer);
	glBindBuffer(GL_TEXTURE_BUFFER, texBuffer);
	glBufferData(GL_TEXTURE_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
	glm::vec4 *positions = (glm::vec4*)glMapBufferRange(GL_TEXTURE_BUFFER, 0, PARTICLE_COUNT * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = glm::vec4(1, 1, 0, 1);
	}
	glUnmapBuffer(GL_TEXTURE_BUFFER);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_BUFFER, tex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, texBuffer);
	
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	
	

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

