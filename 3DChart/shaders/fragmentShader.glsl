#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 gridColor;
uniform float alpha;

uniform bool useTexture;
uniform sampler2D ourTexture;

void main() 
{
    if(useTexture)
        FragColor = vec4(gridColor, alpha);
    else
        FragColor = texture(ourTexture, TexCoord);
}

