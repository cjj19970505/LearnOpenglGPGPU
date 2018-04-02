#version 430 core

layout(local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D inputImage;
layout(r32f, binding = 1) uniform image1D outHistogram;
layout(r32ui, binding = 2) uniform uimage1D uiTest;
struct App
{
	uvec2 imageSize;
};

uniform App app;
//uniform float dt;

void main(void){
    vec4 color = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy));
    //imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(0.1,0.1,0.5,0.1));
	//imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), (color));
	float grayScale = dot(vec4(0.299, 0.587, 0.114, 0), color);
	uvec4 testUI = imageLoad(uiTest, int(gl_GlobalInvocationID.x));
    imageStore(outHistogram, int(gl_GlobalInvocationID.x), vec4(gl_GlobalInvocationID.x/float(app.imageSize.y), 10, 10, 10)*testUI.r);
}