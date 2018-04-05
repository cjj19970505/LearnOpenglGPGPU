#version 430 core

layout(local_size_x = 32, local_size_y = 32) in;
layout(r32f, binding = 0) uniform image2D inputImage;
layout(r32ui, binding = 1) uniform uimage1D outHistogram;
struct App
{
	uvec2 imageSize;
};

uniform App app;
//uniform float dt;

void main(void){
	if(int(gl_GlobalInvocationID.x) < int(app.imageSize.x) && int(gl_GlobalInvocationID.y) < int(app.imageSize.y))
	{
		float grayscale = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy)).r;
		int grayLevel = int(grayscale*255);
		imageAtomicAdd(outHistogram, grayLevel, 1);
	}
}