#version 430

in layout(location=0) vec3 positionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoord;

uniform mat4 fullTransformMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec4 clipSpace;

uniform float textureWidth;

out vec2 horizBlurCoord[11];
out vec2 vertBlurCoord[11];

void main()
{
   vec4 v = vec4(positionModel, 1.0);
   //projected to window viewport position:
   gl_Position = fullTransformMatrix * v;
   clipSpace = gl_Position;


   //projected window viewport x-y-coords:
    vec2 ndc = (gl_Position.xy/gl_Position.w)/2.0 + 0.5;

   float pixelWidth=1.0/textureWidth;

   for (int i=-5;i<=5;i++){
   horizBlurCoord[i+5] = vec2(ndc.x + i*pixelWidth,ndc.y);
   }

      for (int i=-5;i<=5;i++){
   vertBlurCoord[i+5] = vec2(ndc.x,ndc.y + i*pixelWidth);
   }

   //normalWorld = normalize(vec3(modelToWorldTransformMatrix * vec4(normalModel,0))); //doesn't work for nonuniform scaling
   //positionWorld = vec3(modelToWorldTransformMatrix * v);

   //theColor = normalWorld; 
}
