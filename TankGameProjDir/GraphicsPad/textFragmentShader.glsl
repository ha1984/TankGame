#version 430

out vec4 daColor;

in vec2 TexCoord;

uniform sampler2D ourTexture1;

void main()
{

daColor = texture(ourTexture1, TexCoord);//punktweises Produkt

}

