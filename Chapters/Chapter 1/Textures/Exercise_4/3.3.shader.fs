//Use a uniform variable as the mix function's third parameter to vary the amount the two textures are visible. Use the up and down arrow keys to change how much the container or the smiley face is visible

#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float weight;
    
void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), weight);
}