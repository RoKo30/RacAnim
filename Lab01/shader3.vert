#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;

uniform mat4 tModelMatrix;
uniform mat4 tViewMatrix;
uniform mat4 tPerspectiveMatrix;
 

void main()
{
	color = aCol;
    gl_Position = tPerspectiveMatrix * tViewMatrix * tModelMatrix * vec4(aPos, 1.0);
	//gl_Position.z = 0.0;
}