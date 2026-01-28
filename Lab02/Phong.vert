#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aCol;
layout (location = 3) in vec2 aTexCoord;


out vec3 vColor;
out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoord;


uniform mat4 tModelMatrix;
uniform mat4 tViewMatrix;
uniform mat4 tPerspectiveMatrix;
uniform vec4 tEyePos; //


void main()
{
   	vColor = aCol;
    vTexCoord = aTexCoord;
    gl_Position = tViewMatrix * tModelMatrix * vec4(aPos, 1.0); 
    vNormal = normalize(mat3(tViewMatrix) * normalize(mat3(transpose(inverse(tModelMatrix))) * aNormal));
    vFragPos = vec3(tViewMatrix * tModelMatrix * vec4(aPos, 1.0));
}