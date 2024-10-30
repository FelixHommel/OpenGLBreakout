#version 330 core
in vec2 texCoords;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;

out vec4 color;

void main()
{
    color = vec4(0.f);
    vec3 sample[9];

    if(uChaos || uShake)
    {
        for(int i = 0; i < 9; ++i)
            sample[i] = vec3(texture(scene, texCoords.st + offsets[i]));
    }

    if(uChaos)
    {
        for(int i = 0; i < 9; ++i)
            color += vec4(sample[i] * edge_kernel[i], 0.f);

        color.a = 1.f;
    }
    else if(uConfuse)
    {
        color = vec4(1.0 - texture(scene, texCoords).rgb, 1.f);
    }
    else if(uShake)
    {
        for(int i = 0; i < 9; ++i)
            color += vec4(sample[i] * blur_kernel[i], 0.f);

        color.a = 1.f;
    }
    else
    {
        color = texture(scene, texCoords);
    }
}
