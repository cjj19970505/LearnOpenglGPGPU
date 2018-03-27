#version 430 core

layout(local_size_x = 1024) in;
layout(rgba32f, binding = 0) uniform image2D outputImage;
layout(rgba32f, binding = 1) uniform image1D outputArray;


//uniform float dt;

void main(void){
    vec4 color = imageLoad(outputImage, ivec2(gl_GlobalInvocationID.xy));
    imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), vec4(0.1,0.1,0.5,0.1));
    imageStore(outputArray, int(gl_GlobalInvocationID.x), vec4(gl_GlobalInvocationID.x/1024.0f, 0, 0, 1));
}