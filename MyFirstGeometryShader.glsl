#version 440 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

void main() {
    for(int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    gl_Position = vec4(-gl_in[0].gl_Position.x*2, gl_in[2].gl_Position.y, gl_in[0].gl_Position.z, gl_in[0].gl_Position.w);
    EmitVertex();

    EndPrimitive();
}