#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vColor[]; 
in vec3 vNormal[];
in vec3 vFragPos[];
in vec2 vTexCoord[];


out vec3 gColor;  
out vec3 gNormal;  
out vec3 gFragPos;
out vec2 gTexCoord;

uniform vec4 tEyePos; //


uniform mat4 tPerspectiveMatrix;

vec3 getPlaneNormal(vec4 a, vec4 b, vec4 c) {
    vec3 u = vec3(b - a);
    vec3 v = vec3(c - a);
    return normalize(cross(u, v));
}

void main()
{
    vec4 eye = vec4(0.0, 0.0, 0.0, 1.0); // Camera at origin in view space
    vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
    vec3 normal = getPlaneNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

    vec3 toEye = vec3(eye - center);
    if (dot(normal, toEye) > 0.0) {
        for (int i = 0; i < 3; ++i) {
            gColor = vColor[i];
            gNormal = vNormal[i];
            gFragPos = vFragPos[i];
            gTexCoord = vTexCoord[i];
            gl_Position = tPerspectiveMatrix * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}