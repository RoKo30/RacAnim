#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vColor[]; 
out vec3 gColor;  

void main()
{
    // normalize
    vec2 a = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 b = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 c = gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;

    // see if cw or ccw
    float area = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);

    // Emit if ccw
    if (area > 0.0) {
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = gl_in[i].gl_Position;
            gColor = vColor[i]; // Pass color to fragment shader
            EmitVertex();
        }
        EndPrimitive();
    }
}
