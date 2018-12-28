#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float alpha;

void main()
{
    // FragColor = texture(texture2, TexCoord);
    FragColor = vec4(0, 0, texture(texture2, TexCoord).a, 1);
}
