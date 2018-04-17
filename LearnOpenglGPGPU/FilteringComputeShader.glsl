#version 430 core

layout(local_size_x = 32, local_size_y = 32) in;
layout(r32f, binding = 0) uniform image2D filterTemplate;
layout(r32f, binding = 1) uniform image2D inputImage;
layout(r32f, binding = 2) uniform image2D outputImage;
struct App
{
	uvec2 filterSize;
	uvec2 filterCenter;
};

uniform App app;
//uniform float dt;
ivec2 getXAndY(int index)
{
	int rawX = index % int(app.filterSize.x);
	int rawY = index / int(app.filterSize.x);
	return ivec2(rawX, rawY);
}

void main(void){
	float originGrayScale = imageLoad(filterTemplate, ivec2(0, 0)).r;
	int filterLength = int(app.filterSize.x*app.filterSize.y);
	ivec2 originPos = ivec2(gl_GlobalInvocationID.xy);
	float sum = 0;
	int count = 0;
	for(int i = 0; i < filterLength; i++){
		ivec2 filterSamplePos = getXAndY(i);
		float filterFactor = imageLoad(filterTemplate, filterSamplePos).r;
		ivec2 imageSamplePos = filterSamplePos - ivec2(app.filterCenter) + originPos;
		float sampledImage = imageLoad(inputImage, imageSamplePos).r;
		sum += (sampledImage * filterFactor);
		
	}
	//sum /= filterLength;
	float result = abs(sum);
	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(result,0, 0, 0));
	/*
	float sum = 0;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(-1, -1)).r*0;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(0, -1)).r*1;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(1, -1)).r*0;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(-1, 0)).r*1;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(0, 0)).r*(-4);
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(1, 0)).r*1;
	sum += imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)+ivec2(0, 1)).r*1;
	sum = abs(sum);
	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(sum, 0, 0, 0));*/
}

