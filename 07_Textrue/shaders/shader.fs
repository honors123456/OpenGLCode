#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;   //纹理采样器1
uniform sampler2D ourTexture2;  //纹理采样器2

void main()
{
    FragColor = mix(texture(ourTexture1,TexCoord),texture(ourTexture2,TexCoord),0.2);   //mix混合插值,0.2:ourTexture1颜色的80%,ourTexture2颜色的20%
}