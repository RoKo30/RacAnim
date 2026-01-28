#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aCol;

out vec3 vColor;

uniform mat4 tModelMatrix;
uniform mat4 tViewMatrix;
uniform mat4 tPerspectiveMatrix;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 lightPosition;   // In world space
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

void main()
{
    // Transform to view space
    vec3 posView = vec3(tViewMatrix * tModelMatrix * vec4(aPos, 1.0));
    vec3 lightView = vec3(tViewMatrix * vec4(lightPosition, 1.0));
    vec3 normalView = normalize(mat3(tViewMatrix) * normalize(mat3(transpose(inverse(tModelMatrix))) * aNormal));

    // Lighting vectors in view space
    vec3 lightDir = normalize(lightView - posView);
    vec3 viewDir = normalize(-posView); // Camera is at (0,0,0)
    vec3 reflectDir = reflect(-lightDir, normalView);

    // Diffuse and Specular
    float diff = max(dot(normalView, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    vec3 ambient = materialAmbient * lightAmbient;
    vec3 diffuse = materialDiffuse * lightDiffuse * diff;
    vec3 specular = materialSpecular * lightSpecular * spec;

    vec3 intensity = ambient + diffuse + specular;

    vColor = intensity;

    gl_Position = tPerspectiveMatrix * vec4(posView, 1.0);
}
