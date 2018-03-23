#version 330 core
layout (location = 0) in vec3 aPos;


out VS_OUT
{
	vec4 vertexColor;
} vs_out;

struct App
{
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform App app;

void main()
{
    gl_Position = app.projection * app.view * app.model * vec4(aPos, 1.0);
	vs_out.vertexColor = vec4(0,1,0,1);
}