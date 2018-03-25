#version 430 core
out vec4 FragColor;
layout (binding = 0) uniform sampler2D output_image;
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
	FragColor = fs_in.vertexColor;
	FragColor = texture(output_image, vec2(1.0,1.0));
}
