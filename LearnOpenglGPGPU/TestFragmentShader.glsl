#version 430 core
out vec4 FragColor;
layout(rgba32f, binding = 0) uniform imageBuffer testImageBuffer;

in VS_OUT
{
	vec4 vertexColor;
} fs_in;

struct App
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;
void main()
{
	vec4 pos = imageLoad(testImageBuffer, 0);
	FragColor = fs_in.vertexColor;
	FragColor = pos;
	int PARTICLE_COUNT = 100;
	for(int i = 0; i<PARTICLE_COUNT; i++){
		imageStore(testImageBuffer, PARTICLE_COUNT, vec4(2,2,2,2));
	}
	
}
