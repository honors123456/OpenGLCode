#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 gridColor;
uniform float alpha;

uniform bool useTexture;
uniform sampler2D ourTexture;
uniform vec3 textColor;

void main() 
{
    if(useTexture)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, TexCoord).r);
        FragColor = vec4(textColor, 0.8) * sampled;
    }
    else
        FragColor = vec4(gridColor, alpha);
}

