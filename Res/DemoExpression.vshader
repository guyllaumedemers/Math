#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) uniform mat4 projMat;
layout (location = 2) uniform mat4 modelviewMat;

void main()
{
    gl_Position = projMat * modelviewMat * vec4(aPos, 1.0);
}