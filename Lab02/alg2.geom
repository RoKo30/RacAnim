#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vColor[]; 
in vec4 vLightPos[];

out vec3 gColor;  

uniform mat4 tPerspectiveMatrix;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform vec4 tEyePos; //

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

vec3 getPlaneNormal(vec4 a, vec4 b, vec4 c) {
    vec3 u = vec3(b - a);
    vec3 v = vec3(c - a);
    return normalize(cross(u, v));
}

void main()
{
    vec4 c = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
    vec4 e = tEyePos - c;

    vec3 normal = getPlaneNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
    vec3 lightDir = normalize(vec3(vLightPos[0] - c));
    vec3 viewDir = normalize(vec3(e));
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    if (dot(vec3(e), normal) > 0.0) {
        vec3 intensity = materialAmbient * lightAmbient + lightDiffuse * materialDiffuse * diff + lightSpecular * materialSpecular * spec;
        for (int i = 0; i < 3; ++i) {
            gl_Position = tPerspectiveMatrix * gl_in[i].gl_Position;
            gColor = intensity; // Pass color to fragment shader
            EmitVertex();
        }
    }
    EndPrimitive();
}
