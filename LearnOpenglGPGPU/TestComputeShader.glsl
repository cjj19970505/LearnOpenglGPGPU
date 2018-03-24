#version 430 core

layout(local_size_x = 128) in;
layout(rgba32f, binding = 0) uniform imageBuffer velocity_buffer;


//uniform float dt;

void main(void){
    
    vec4 pos = imageLoad(velocity_buffer, int(gl_GlobalInvocationID.x));
    pos += vec4(gl_GlobalInvocationID, 0);
    pos = vec4(2,2,2,2);
    imageStore(velocity_buffer, int(gl_GlobalInvocationID.x), pos);
}