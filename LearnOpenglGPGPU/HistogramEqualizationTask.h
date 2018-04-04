#pragma once
#include "ComputeTask.h"
#include "ComputeShader.h"
#include <glad\glad.h>
#include "HistogramComputeTask.h"

///����һ��ͼ��RGB�����ֱ��ͼ���⻯�Ľ����ֻ�лҶȣ�
class HistogramEqualizationTask :
	public ComputeTask
{
private:
	GLuint sourceTextureId = 0;
	HistogramComputeTask *histogramComputeTask;
	int width = 0;
	int height = 0;
public:
	HistogramEqualizationTask();
	~HistogramEqualizationTask();
	bool isConfigCompleted()
	{
		if (glIsTexture(sourceTextureId))
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
		this->width = width;
		this->height = height;
		histogramComputeTask->setSourceTexture(textureId, width, height);
		
	}
};



HistogramEqualizationTask::HistogramEqualizationTask()
{
	histogramComputeTask = new HistogramComputeTask();
}


HistogramEqualizationTask::~HistogramEqualizationTask()
{
}
