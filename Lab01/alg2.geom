#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in vec4 eyePos[];
uniform mat4 tPerspectiveMatrix;

vec3 getPlaneNormal(vec4 a, vec4 b, vec4 c) {
    vec3 u = vec3(b - a);
    vec3 v = vec3(c - a);
    return normalize(cross(u, v));
}

void main()
{
    vec4 c = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
    vec4 e = (eyePos[0] + eyePos[1] + eyePos[2]) / 3.0 - c;

    vec3 normal = getPlaneNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

    if (dot(vec3(e), normal) > 0.0) {
        for (int i = 0; i < 3; ++i) {
            gl_Position = tPerspectiveMatrix * gl_in[i].gl_Position;
            EmitVertex();
        }
    }
    EndPrimitive();
}
