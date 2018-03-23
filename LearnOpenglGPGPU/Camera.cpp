#include "stdafx.h"
#include "Camera.h"
#include <iostream>
using namespace xjg;
using namespace std;
void Camera::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (isFirstInput)
	{
		lastMousePos = glm::vec2(xpos, ypos);
		isFirstInput = false;
	}
	glm::vec2 currMousePos = glm::vec2(xpos, ypos);
	glm::vec2 deltaMousePos = currMousePos - lastMousePos;
	rotation.x += deltaMousePos.y*mouseSensitive;
	rotation.y += deltaMousePos.x*mouseSensitive;
	lastMousePos = currMousePos;

}
void Camera::processInput(GLFWwindow *window)
{
	glm::vec3 cameraFront;
	glm::vec3 cameraUp(0, 1, 0);
	cameraFront.x = cos(glm::radians(-rotation.x))*cos(glm::radians(rotation.y));
	cameraFront.y = sin(glm::radians(-rotation.x));
	cameraFront.z = cos(glm::radians(-rotation.x))*sin(glm::radians(rotation.y));
	cameraFront = glm::normalize(cameraFront);
	glm::vec3 deltaPos(0, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		deltaPos.x += moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		deltaPos.x -= moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		deltaPos.z -= moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		deltaPos.z += moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		fov += fovSpeed;
		if (fov >= 180)
		{
			fov = 180;
		}
		cout << "FOV:" << fov << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		fov -= fovSpeed;
		if (fov <= 0)
		{
			fov = 0;
		}
		cout << "FOV:" << fov << endl;
	}
	glm::vec3 finalDelta = cameraFront * deltaPos.x + glm::cross(cameraFront, cameraUp)*deltaPos.z;
	position += finalDelta;

}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 view;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp(0, 1, 0);
	cameraFront.x = cos(glm::radians(-rotation.x))*cos(glm::radians(rotation.y));
	cameraFront.y = sin(glm::radians(-rotation.x));
	cameraFront.z = cos(glm::radians(-rotation.x))*sin(glm::radians(rotation.y));
	cameraFront = glm::normalize(cameraFront);
	view = glm::lookAt(position, position + cameraFront, cameraUp);
	//std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
	return view;
}
Camera::~Camera()
{
}