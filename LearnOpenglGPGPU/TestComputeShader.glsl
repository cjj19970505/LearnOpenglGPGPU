#version 430 core

layout(local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D inputImage;
layout(rgba32f, binding = 1) uniform image2D outputImage;
layout(r32f, binding = 2) uniform image1D outputArray;


//uniform float dt;

void main(void){
    vec4 color = imageLoad(inputImage, ivec2(gl_GlobalInvocationID.xy));
    //imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(0.1,0.1,0.5,0.1));
	imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), sin(color*2*3.1415926));
    imageStore(outputArray, int(gl_GlobalInvocationID.x), vec4(1-gl_GlobalInvocationID.x/1024.0f, 1-gl_GlobalInvocationID.x/1024.0f, 0, 1));
}