#version 430 core

layout(local_size_x = 32, local_size_y = 32) in;
layout(rgba32f, binding = 0) uniform image2D inputImage;
layout(r32f, binding = 1) uniform image2D outputImage;

float grayscale(vec4 color)
{
	return color.r*0.299+color.g*0.587+color.b*0.114;
} 
void main(void){
	vec4 color = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy));
	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(grayscale(color),0,0,0));
}