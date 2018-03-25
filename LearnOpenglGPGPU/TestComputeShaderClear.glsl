#version 430 core

layout(local_size_x = 1024) in;
layout(rgba32f, binding = 0) uniform image2D outputImage;


//uniform float dt;

void main(void){
    vec4 color = imageLoad(outputImage, ivec2(gl_GlobalInvocationID.xy));
    imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), color+vec4(0.1,0.1,0.1,0.1));
}