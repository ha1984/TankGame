#version 430

in layout(location=0) vec3 positionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoord;

out vec2 TexCoord;

uniform float cursorX;
uniform float cursorY;

void main()
{
   vec4 v = vec4(positionModel.x + cursorX,positionModel.y - cursorY, positionModel.z, 1.0);
   gl_Position = v;
   
   TexCoord = vec2(texCoord.x, 1.0 - (texCoord.y));
}
