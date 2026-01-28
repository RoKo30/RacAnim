#version 330 core
in vec3 gColor;
in vec3 gNormal;
in vec3 gFragPos;
in vec2 gTexCoord;
uniform sampler2D diffuseTexture;

out vec4 FragColor;

uniform mat4 tViewMatrix;

uniform vec3 lightPosition;     // In view space
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

void main()
{
    vec4 lightPos = tViewMatrix * vec4(lightPosition, 1.0);
    vec3 norm = normalize(gNormal);
    vec3 lightDir = normalize(vec3(lightPos) - gFragPos);
    vec3 viewDir = normalize(-gFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    vec3 ambient = lightAmbient * materialAmbient;
    vec3 diffuse = lightDiffuse * materialDiffuse * diff;
    vec3 specular = lightSpecular * materialSpecular * spec;

    vec3 texColor = texture(diffuseTexture, gTexCoord).rgb;
    vec3 result = (ambient + diffuse + specular) * texColor;
    //FragColor = vec4(result, 1.0);
    //FragColor = vec4(gTexCoord, 0.0, 1.0); // Should produce a gradient if UVs are working

    //vec3 result = (ambient + diffuse + specular); // * gColor;

    FragColor = vec4(result, 1.0);
}
