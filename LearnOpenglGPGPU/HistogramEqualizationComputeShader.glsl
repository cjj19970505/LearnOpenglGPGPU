#version 430 core

layout(local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D inputImage;
layout(r32ui, binding = 1) uniform uimage1D inHistogram;
struct App
{
	uvec2 imageSize;
};

uniform App app;
//uniform float dt;

void main(void){
    vec4 color = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy));
	uvec4 testUI = imageLoad(uiTest, int(gl_GlobalInvocationID.x));
	//testUI = uvec4(1,1,1,1);
    //imageStore(outHistogram, int(gl_GlobalInvocationID.x), vec4(gl_GlobalInvocationID.x/float(app.imageSize.y), 10, 10, 10)*testUI);
	float grayScale = color.r*0.299+color.g*0.587+color.b*0.114;
	int grayLevel = int(grayScale*255);
	imageAtomicAdd(outHistogram, grayLevel, 1);
	//imageStore(outHistogram, int(gl_GlobalInvocationID.x), vec4(gl_GlobalInvocationID.x/float(app.imageSize.y), 10, 10, 10)*testUI);
}