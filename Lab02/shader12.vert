#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aCol;

out vec3 vColor;
out vec4 vLightPos;    //

uniform mat4 tModelMatrix;
uniform mat4 tViewMatrix;
uniform mat4 tPerspectiveMatrix;
uniform vec4 tEyePos; //
 
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

void main()
{
   	vColor = aCol;
    vLightPos = tViewMatrix * vec4(lightPosition, 1.0); 
    gl_Position = tViewMatrix * tModelMatrix * vec4(aPos, 1.0); 
    //gl_Position = tPerspectiveMatrix * tViewMatrix * tModelMatrix * vec4(aPos, 1.0);
	//gl_Position.z = 0.0;
}