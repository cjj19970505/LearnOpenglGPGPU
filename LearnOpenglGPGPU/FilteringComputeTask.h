#pragma once
#include "ComputeTask.h"
#include "ComputeShader.h"
#include <glad\glad.h>


class FilteringComputeTask:
	public ComputeTask
{
private:
	GLuint filterTemplateTextureId;
	int filterTemplateWidth;
	int filterTemplateHeight;
	int filterCenterX;
	int filterCenterY;
	ComputeShader *filteringComputeShader;

	bool computeCompleted = false;
	int width = 0;
	int height = 0;
	
	GLuint sourceTextureId = 0;
	GLuint outputTextureId = 0;
	
public:
	FilteringComputeTask();
	void setFilter(float *filter, int filterTemplateWidth, int filterTemplateHeight, int centerX, int centerY)
	{
		if (glIsTexture(this->filterTemplateTextureId))
		{
			glDeleteTextures(1, &(this->filterTemplateTextureId));
			filterTemplateTextureId = 0;
		}
		this->filterTemplateWidth = filterTemplateWidth;
		this->filterTemplateHeight = filterTemplateHeight;
		this->filterCenterX = centerX;
		this->filterCenterY = centerY;
		glGenTextures(1, &filterTemplateTextureId);
		glBindTexture(GL_TEXTURE_2D, filterTemplateTextureId);
		glTexStorage2D(GL_TEXTURE_2D, 8, GL_R32F, filterTemplateWidth, filterTemplateHeight);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, filterTemplateWidth, filterTemplateHeight, 0, GL_RED, GL_FLOAT, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
	void setSourceTexture(GLuint sourceTextureId, int width, int height)
	{
		computeCompleted = false;
		this->sourceTextureId = sourceTextureId;
		if (!(width == this->width && height == this->height))
		{
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
	~FilteringComputeTask();
	bool isConfigCompleted()
	{
		if (glIsTexture(sourceTextureId) && glIsTexture(outputTextureId) && glIsTexture(filterTemplateTextureId))
		{
			return true;
		}
		else
		{
			cout << "FiltringComputeTask isn't complete" << endl;
			return false;
		}
	}
	bool execute()
	{
		if (!isConfigCompleted())
		{
			return false;
		}
		filteringComputeShader->use();
		filteringComputeShader->setUvec2("app.filterSize", glm::uvec2(filterTemplateWidth, filterTemplateHeight));
		filteringComputeShader->setUvec2("app.filterCenter", glm::uvec2(filterCenterX, filterCenterY));
		glBindImageTexture(0, filterTemplateTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
		glBindImageTexture(1, sourceTextureId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
		glBindImageTexture(2, outputTextureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
		glDispatchCompute(width / 32 + 1, height / 32 + 1, 1);
		return true;
	}
	GLuint getOutputTextureId()
	{
		return outputTextureId;
	}
};



FilteringComputeTask::FilteringComputeTask()
{
	filteringComputeShader = new ComputeShader("FilteringComputeShader.glsl");
}



FilteringComputeTask::~FilteringComputeTask()
{
}
