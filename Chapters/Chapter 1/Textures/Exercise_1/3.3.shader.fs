#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 flipped = vec2(-TexCoord.x, TexCoord.y);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, flipped), 0.2);
}