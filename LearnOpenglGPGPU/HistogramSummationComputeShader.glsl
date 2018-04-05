#version 430 core

layout(local_size_x = 1) in;
layout(r32ui, binding = 0) uniform uimage1D inHistogram;
layout(r32ui, binding = 1) uniform uimage1D outSummedHistogram;

void main(void){
	
	uint sum = 0;
	for(int i = 0; i <= int(gl_GlobalInvocationID.x); i++)
	{
		sum += imageLoad(inHistogram, i).r;
	}
	//imageStore(inHistogram, int(gl_GlobalInvocationID.x), uvec4(0,0,0,0));
	imageStore(outSummedHistogram, int(gl_GlobalInvocationID.x), uvec4(sum,0,0,0));
}