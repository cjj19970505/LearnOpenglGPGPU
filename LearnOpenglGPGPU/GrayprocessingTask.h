#pragma once
#include "ComputeTask.h"
#include "ComputeShader.h"
#include <glad\glad.h>

//灰度化一张图像
class GrayprocessingTask
{
private:

	//GL_TEXTURE_2D, GL_RGBA, GL_RGBA32F
	GLuint sourceTextureId = 0;
	int width = 0;
	int height = 0;
	ComputeShader *grayprocessingComputeShader;

	//GL_TEXTURE_2D, GL_RED, GL_R32F
	GLuint outputTextureId;
public:
	GrayprocessingTask();
	void setTexture(GLuint texture, int width, int height);
	bool execute();
	bool isConfigCompleted();
	GLuint getOutputTextureId()
	{
		return outputTextureId;
	}
	~GrayprocessingTask();
};



GrayprocessingTask::GrayprocessingTask()
{
	grayprocessingComputeShader = new ComputeShader("GrayprocessingComputeShader.glsl");
}

inline bool GrayprocessingTask::isConfigCompleted()
{
	if (glIsTexture(sourceTextureId))
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline void GrayprocessingTask::setTexture(GLuint textureId, int width, int height)
{
	this->sourceTextureId = textureId;
	this->width = width;
	this->height = height;

	if (glIsTexture(outputTextureId))
	{
		glDeleteTextures(1, &outputTextureId);
	}
	GLuint outputTextureId;
	glGenTextures(1, &outputTextureId);
	glBindTexture(GL_TEXTURE_2D, outputTextureId);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	this->outputTextureId = outputTextureId;
}

inline bool GrayprocessingTask::execute()
{
	if (!isConfigCompleted())
	{
		cout << "SOMETHINGWRONG" << endl;
		return false;
	}
	grayprocessingComputeShader->use();
	glBindImageTexture(0, sourceTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, outputTextureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	glDispatchCompute(width/32+1, height/32+1, 1);
	return true;
}


GrayprocessingTask::~GrayprocessingTask()
{
}
