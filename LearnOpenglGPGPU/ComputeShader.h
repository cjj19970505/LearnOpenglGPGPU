#pragma once
#include <string>
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using namespace std;
class ComputeShader
{
private:
	string path;
	bool loaded;
	bool error;
	GLuint id;
	string loadFile(string filePath);
public:
	ComputeShader(string path) :loaded(false), path(path), error(false), id(0){}
	void load();
	void use();
	void setUvec2(const std::string & name, glm::uvec2 value) const
	{
		glUniform2ui(glGetUniformLocation(id, name.c_str()), value.x, value.y);
	}
	~ComputeShader();
};

