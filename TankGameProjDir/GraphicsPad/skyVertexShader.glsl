#version 430

in layout(location=0) vec3 positionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoord;

uniform vec3 ambientLight;

uniform mat4 fullTransformMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec3 positionWorld;
out vec3 normalWorld;
out vec2 TexCoord;
out vec3 theColor;

out vec3 ambient;

uniform vec4 plane; //clip off everything under this plane

void main()
{
   vec4 v = vec4(positionModel, 1.0);
   gl_Position = fullTransformMatrix * v;

   //normalWorld = normalize(vec3(modelToWorldTransformMatrix * vec4(normalModel,0))); //doesn't work for nonuniform scaling
   //positionWorld = vec3(modelToWorldTransformMatrix * v);

   gl_ClipDistance[0] = -dot(vec4(positionWorld,1.0),plane); 

   ambient = ambientLight;

   TexCoord = vec2(texCoord.x, 1.0 - (texCoord.y));
   
  // theColor = normalWorld; 
}
