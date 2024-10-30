#version 330 core
layout(location = 0) in vec4 vertex;

uniform mat4 uModel;
uniform mat4 uProjection;

out vec2 TexCoord;

void main()
{
    TexCoord = vertex.zw;

    gl_Position = uProjection * uModel * vec4(vertex.xy, 0.f, 1.f);
}

