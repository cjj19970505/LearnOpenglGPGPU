#version 430 core

layout(local_size_y = 1) in;
layout(r32ui, binding = 0) uniform uimage1D inHistogram;

void main(void){
	imageStore(inHistogram, int(gl_GlobalInvocationID.x), uvec4(0,0,0,0));
}