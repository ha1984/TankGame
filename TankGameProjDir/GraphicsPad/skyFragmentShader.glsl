#version 430

out vec4 daColor;

in vec3 positionWorld;
in vec3 normalWorld;
in vec2 TexCoord;
in vec3 theColor;

in vec3 ambient;

uniform sampler2D ourTexture1;

void main()
{
vec3 light = ambient;
//daColor =  texture(shadowPosX, TexCoord);
//daColor = vec4(theColor, 1.0);
daColor = texture(ourTexture1, TexCoord) * vec4(light, 1.0);//punktweises Produkt
//gl_FragDepth = gl_FragCoord.z;
}

