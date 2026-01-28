#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 vColor;
out vec4 eyePos;    

uniform mat4 tModelMatrix;
uniform mat4 tViewMatrix;
uniform vec4 tEyePos;

void main()
{
    vColor = aCol;
    eyePos = tEyePos; 
    gl_Position = tViewMatrix * tModelMatrix * vec4(aPos, 1.0); 
}
