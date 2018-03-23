#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GLFW\glfw3.h>

namespace xjg
{
	class Camera
	{
	private:

		glm::vec2 lastMousePos;
		bool isFirstInput;
	public:
		float mouseSensitive = 0.02f;
		float moveSpeed = 0.02f;
		float fovSpeed = 2.0f;
		float fov = 30;
		glm::vec3 position;
		glm::vec3 rotation;
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		Camera(glm::vec3 pos, glm::vec3 rot) :position(pos), rotation(rot), isFirstInput(true) {};
		void processInput(GLFWwindow *window);
		glm::mat4 getViewMatrix();
		~Camera();
	};
}
