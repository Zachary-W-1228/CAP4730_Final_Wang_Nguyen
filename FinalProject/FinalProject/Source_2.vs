#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position = proj * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   FragPos = vec3(transform * vec4(aPos, 1.0));
   Normal =  mat3(transpose(inverse(transform))) * aNormal;
}