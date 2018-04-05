#pragma once
#include "ComputeTask.h"
#include "ComputeShader.h"
#include <glad\glad.h>
class HistogramComputeTask :
	public ComputeTask
{
private:
	//回去后改成智能指针
	ComputeShader *histogramComputeShader;
	ComputeShader *clearHistogramArrayComputeShader;

	
	//GL_TEXTURE_2D, GL_RED, GL_R32F 灰度图！！
	GLuint sourceTextureId = 0;
	GLuint histogramArrayId = 0;
	
	int width = 0;
	int height = 0;
	int grayscaleLevel = 256;
	bool computeCompleted = false;
public:
	HistogramComputeTask(int grayscaleLevel = 256)
	{
		histogramComputeShader = new ComputeShader("HistogramComputeShader.glsl");
		clearHistogramArrayComputeShader = new ComputeShader("ClearHistogramArrayComputeShader.glsl");
		setGrayscaleLevel(grayscaleLevel);
	}
	void setSourceTexture(GLuint sourceTextureId, int width, int height)
	{
		computeCompleted = false;
		this->sourceTextureId = sourceTextureId;
		this->width = width;
		this->height = height;
	}
	GLuint getHistogramArrayId()
	{
		return histogramArrayId;
	}
	bool isConfigCompleted()
	{
		if (glIsTexture(histogramArrayId) && glIsTexture(sourceTextureId))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void setGrayscaleLevel(int grayscaleLevel)
	{
		computeCompleted = false;
		this->grayscaleLevel = grayscaleLevel;
		if (glIsTexture(this->histogramArrayId))
		{
			glDeleteTextures(1, &(this->histogramArrayId));
		}
		glGenTextures(1, &histogramArrayId);
		glBindTexture(GL_TEXTURE_1D, histogramArrayId);
		glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32UI, grayscaleLevel);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_1D, 0);
		
	}

	//返回值表明是否执行成功
	bool execute()
	{
		if (!isConfigCompleted())
		{
			return false;
		}
		//先清除之前的直方图
		clearHistogramArrayComputeShader->use();
		glBindImageTexture(0, histogramArrayId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
		glDispatchCompute(1, 1, 1);

		histogramComputeShader->use();
		histogramComputeShader->setUvec2("app.imageSize", glm::uvec2(width, height));
		glBindImageTexture(0, sourceTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
		glBindImageTexture(1, histogramArrayId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
		glDispatchCompute(width/32+1, height/32+1, 1);
		return true;
	}
	~HistogramComputeTask()
	{

	}
	
};

