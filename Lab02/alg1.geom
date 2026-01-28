#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 eyePos[];
in vec3 vColor[]; 
out vec3 gColor;  

uniform mat4 tPerspectiveMatrix;

vec3 getPlaneNormal(vec4 a, vec4 b, vec4 c) {
    vec3 u = vec3(b - a);
    vec3 v = vec3(c - a);
    return normalize(cross(u, v));
}

void main()
{
    vec4 e = eyePos[0];
    vec3 normal = getPlaneNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
    float d = - normal.x * gl_in[0].gl_Position.x - normal.y * gl_in[0].gl_Position.y - normal.z * gl_in[0].gl_Position.z;


    if (normal.x * e.x + normal.y * e.y + normal.z * e.z + d > 0.0) {
        for (int i = 0; i < 3; ++i) {
            gl_Position = tPerspectiveMatrix * gl_in[i].gl_Position;
            gColor = vColor[i]; // Pass color to fragment shader
            EmitVertex();
        }
    }
    EndPrimitive();
}
