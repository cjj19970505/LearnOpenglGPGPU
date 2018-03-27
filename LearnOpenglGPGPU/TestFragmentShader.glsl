#version 430 core
out vec4 FragColor;
layout (binding = 0) uniform sampler2D output_image;
layout (binding = 0) uniform sampler1D output_array;

in VS_OUT
{
	vec2 texCoords;
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
	float coord = ((fs_in.vertexColor + vec4(1,1,1,1))/2).y;
	//FragColor = ((fs_in.vertexColor + vec4(1,1,1,1))/2).yyyy ;
	//FragColor = texture(output_image, vec2(1,1));
	FragColor = texture1D(output_array, coord);
}
