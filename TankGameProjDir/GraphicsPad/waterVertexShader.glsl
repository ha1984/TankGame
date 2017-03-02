#version 430

in layout(location=0) vec3 positionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoord;

uniform mat4 fullTransformMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec3 positionWorld;
out vec3 normalWorld;
out vec3 theColor;

out vec4 clipSpace;

out vec2 textureCoord;

void main()
{
   textureCoord = vec2(positionModel.x/2.0,positionModel.z/2.0);
   vec4 v = vec4(positionModel, 1.0);
   //projected to window viewport position:
   gl_Position = fullTransformMatrix * v;
   clipSpace = gl_Position;

   normalWorld = normalize(vec3(modelToWorldTransformMatrix * vec4(normalModel,0))); //doesn't work for nonuniform scaling
   positionWorld = vec3(modelToWorldTransformMatrix * v);

   theColor = normalWorld; 
}
