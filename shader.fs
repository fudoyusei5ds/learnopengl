#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float alpha;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), alpha);
    FragColor = vec4(texture(texture2, TexCoord).r,texture(texture2, TexCoord).g,texture(texture2, TexCoord).b,texture(texture2, TexCoord).r);
}
