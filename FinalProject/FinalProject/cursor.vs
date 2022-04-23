#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position = proj * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   gl_Position.z = 0.0f;
}