#version 440 core
layout (location = 0) in vec4 offset;   

void main()
{
    const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0), vec4(-0.25, -0.25, 0.5, 1.0), vec4(0.25, 0.25, 0.5, 1.0));
    gl_Position = vertices[gl_VertexID] + offset;
}