#include "stdafx.h"
#include "ComputeShader.h"
#include "stdafx.h"
#include <glad\glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;



void ComputeShader::load()
{
	if (loaded)
	{
		return;
	}
	string cShaderCode1 = loadFile(path);
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
		std::cout << "ERROR::SHADER::COMPUTE_SHADER("<<path<<")::COMPILATION_FAILED\n" << infoLog << std::endl;
		error = true;
		return;
	}
	GLuint computeProg1 = glCreateProgram();
	glAttachShader(computeProg1, cShader1);
	glLinkProgram(computeProg1);
	glGetProgramiv(computeProg1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(computeProg1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteShader(cShader1);
		error = true;
		return;
	}
	glDeleteShader(cShader1);
	this->id = computeProg1;
	loaded = true;
}
void ComputeShader::use()
{
	if (error)
	{
		return;
	}
	if (id == 0 || !loaded )
	{
		load();
	}
	if (!error)
	{
		glUseProgram(id);
	}
	
}
string ComputeShader::loadFile(string filePath)
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
ComputeShader::~ComputeShader()
{
}
