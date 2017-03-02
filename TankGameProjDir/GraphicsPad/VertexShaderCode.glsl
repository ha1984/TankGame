#version 430

in layout(location=0) vec3 positionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoord;

uniform vec3 ambientLight;

uniform mat4 fullTransformMatrix;
uniform mat4 modelToWorldTransformMatrix;
uniform mat4 worldToViewMatrix;

uniform mat4 worldToProjectionPosX;
uniform mat4 worldToProjectionNegX;
uniform mat4 worldToProjectionPosY;
uniform mat4 worldToProjectionNegY;
uniform mat4 worldToProjectionPosZ;
uniform mat4 worldToProjectionNegZ;

uniform vec2 texShift;

out vec3 positionWorld;
out vec3 normalWorld;
out vec2 TexCoord;
out vec3 theColor;

out vec4 clipSpacePosX;
out vec4 clipSpaceNegX;
out vec4 clipSpacePosY;
out vec4 clipSpaceNegY;
out vec4 clipSpacePosZ;
out vec4 clipSpaceNegZ;

out vec3 ambient;
//for fog:
out float visibility;
float density= 0.03;
float gradient = 4.0;


uniform vec4 plane; //clip off everything above this plane

void main()
{
   vec4 v = vec4(positionModel, 1.0);
   gl_Position = fullTransformMatrix * v;

   normalWorld = normalize(vec3(modelToWorldTransformMatrix * vec4(normalModel,0))); //doesn't work for nonuniform scaling
   positionWorld = vec3(modelToWorldTransformMatrix * v);

    gl_ClipDistance[0] = -dot(vec4(positionWorld,1.0),plane);

   vec4 positionToCam =  worldToViewMatrix * vec4(positionWorld,1.0);

   float distance = length(positionToCam.xyz);
   visibility = exp(-pow(distance*density,gradient));
   visibility = clamp(visibility,0.0,1.0);

   ambient = ambientLight;

   TexCoord = vec2(texCoord.x + texShift.x, 1.0 - (texCoord.y+texShift.y));
   
   theColor = normalWorld; 

   clipSpacePosX = worldToProjectionPosX * vec4(positionWorld,1.0);
   clipSpaceNegX = worldToProjectionNegX * vec4(positionWorld,1.0);
   clipSpacePosY = worldToProjectionPosY * vec4(positionWorld,1.0);
   clipSpaceNegY = worldToProjectionNegY * vec4(positionWorld,1.0);
   clipSpacePosZ = worldToProjectionPosZ * vec4(positionWorld,1.0);
   clipSpaceNegZ = worldToProjectionNegZ * vec4(positionWorld,1.0);
}
