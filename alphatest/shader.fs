#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float alpha;

void main()
{
    FragColor=vec4(texture(texture1, TexCoord).r, texture(texture1, TexCoord).g, texture(texture1, TexCoord).b, texture(texture2, TexCoord).r);
}
