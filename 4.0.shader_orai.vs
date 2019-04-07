#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform mat4 trans;

void main()
{
   gl_Position =  trans * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourColor = aColor;
};
