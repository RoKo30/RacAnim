#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aCol;

out vec3 vColor;

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
    vec4 LightPos = tViewMatrix * vec4(lightPosition, 1.0);
    vec4 e = tEyePos - vec4(aPos, 1.0);

    vec3 lightDir = normalize(vec3(LightPos - vec4(aPos, 1.0)));
    vec3 viewDir = normalize(vec3(e));
    vec3 reflectDir = reflect(-lightDir, aNormal);

    float diff = max(dot(aNormal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    vec3 intensity = materialAmbient * lightAmbient + lightDiffuse * materialDiffuse * diff + lightSpecular * materialSpecular * spec;

	vColor = intensity;
    gl_Position = tPerspectiveMatrix * tViewMatrix * tModelMatrix * vec4(aPos, 1.0); 
}