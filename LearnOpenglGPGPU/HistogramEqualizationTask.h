#pragma once
#include "ComputeTask.h"
#include "ComputeShader.h"
#include <glad\glad.h>
#include "HistogramComputeTask.h"

///输入一副图像RGB，输出直方图均衡化的结果（只有灰度）
class HistogramEqualizationTask :
	public ComputeTask
{
private:
	GLuint summedHistogramArrayId = 0;
	//GL_TEXTURE_2D, GL_RED, GL_R32F 灰度图！！
	GLuint sourceTextureId = 0;
	GLuint outputTextureId = 0;
	HistogramComputeTask *histogramComputeTask;
	//这货是用来将直方图做一个求和的
	ComputeShader *histogramSummationComputeShader;
	ComputeShader *histogramEqualizationShader;

	int width = 0;
	int height = 0;
public:
	HistogramEqualizationTask();
	~HistogramEqualizationTask();
	bool isConfigCompleted()
	{
		if (glIsTexture(sourceTextureId) && glIsTexture(outputTextureId))
		{
			return histogramComputeTask->isConfigCompleted();
		}
		else
		{
			return false;
		}
	}
	void setTexture(GLuint textureId, int width, int height)
	{
		this->sourceTextureId = textureId;
		histogramComputeTask->setSourceTexture(textureId, width, height);
		if (!(width == this->width && height == this->height))
		{
			if (glIsTexture(this->outputTextureId))
			{
				glDeleteTextures(1, &outputTextureId);
				outputTextureId = 0;
			}
			GLuint outputImage;
			glGenTextures(1, &outputImage);
			glBindTexture(GL_TEXTURE_2D, outputImage);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, width, height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
			this->outputTextureId = outputImage;
		}
		this->width = width;
		this->height = height;
		
	}
	GLuint getOutputTextureId()
	{
		return outputTextureId;
	}
	bool execute()
	{
		if (!isConfigCompleted())
		{
			return false;
		}
		bool histogramSucceed = histogramComputeTask->execute();
		if (!histogramSucceed)
		{
			return false;
		}
		histogramSummationComputeShader->use();
		glBindImageTexture(0, histogramComputeTask->getHistogramArrayId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
		glBindImageTexture(1, summedHistogramArrayId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
		glDispatchCompute(256, 1, 1);
		histogramEqualizationShader->use();

		glBindImageTexture(0, summedHistogramArrayId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
		glBindImageTexture(1, sourceTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
		glBindImageTexture(2, outputTextureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
		histogramEqualizationShader->setUvec2("app.imageSize", glm::uvec2(width, height));
		glDispatchCompute(width/32+1, height/32+1, 1);
		return true;
	}
};



HistogramEqualizationTask::HistogramEqualizationTask()
{
	histogramComputeTask = new HistogramComputeTask();
	histogramEqualizationShader = new ComputeShader("HistogramEqualizationComputeShader.glsl");
	histogramSummationComputeShader = new ComputeShader("HistogramSummationComputeShader.glsl");

	glGenTextures(1, &summedHistogramArrayId);
	glBindTexture(GL_TEXTURE_1D, summedHistogramArrayId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32UI, 256);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_1D, 0);
}


HistogramEqualizationTask::~HistogramEqualizationTask()
{
}
