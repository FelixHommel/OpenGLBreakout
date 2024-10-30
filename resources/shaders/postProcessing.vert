#version 330 core
layout (location = 0) in vec4 vertex;

uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;
uniform float uTime;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(vertex.xy, 0.f, 1.f);
    vec2 texture = vertex.zw;

    if(uChaos)
    {
        float strength = 0.3f;
        vec2 pos = vec2(texture.x + sin(uTime) * strength, texture.y + cos(uTime) * strength);
        texCoords = pos;
    }
    else if(uConfuse)
    {
        texCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
    }
    else
    {
        texCoords = texture;
    }

    if(uShake)
    {
        float strength = 0.01f;

        gl_Position.x += cos(uTime * 10) * strength;
        gl_Position.y += cos(uTime * 15) * strength;
    }
}
