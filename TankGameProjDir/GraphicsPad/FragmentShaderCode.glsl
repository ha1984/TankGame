#version 430

out vec4 daColor;

in vec3 positionWorld;
in vec3 normalWorld;
in vec2 TexCoord;
in vec3 theColor;

in vec3 ambient;
uniform vec3 specularity;
uniform float alpha;

uniform sampler2D ourTexture1;
uniform sampler2D shadowPosX;
uniform sampler2D shadowNegX;
uniform sampler2D shadowPosY;
uniform sampler2D shadowNegY;
uniform sampler2D shadowPosZ;
uniform sampler2D shadowNegZ;
uniform vec3 eyePosition;
uniform vec3 lightPosition;

in vec4 clipSpacePosX;
in vec4 clipSpaceNegX;
in vec4 clipSpacePosY;
in vec4 clipSpaceNegY;
in vec4 clipSpacePosZ;
in vec4 clipSpaceNegZ;

float pi = 3.14159;
float zNear=0.1f;
float zFar=150.0f;
float depthBias = 0.2f; //increasing depthBias (or zFar) produces more light -> reduces shadow acne

//for fog;
in float visibility;
uniform vec3 fogColor;

void main()
{
   vec3 r = lightPosition - positionWorld;
   vec3 lightVector = normalize(r);

   vec3 R = -r;
   float distance;
   
   vec2 ndc;
   float z_b;

   if ((R.x > zNear) && (abs(R.y) < R.x) && (abs(R.z) < R.x)) {
   ndc = (clipSpacePosX.xy/clipSpacePosX.w)/2.0 + 0.5;
   z_b = texture2D(shadowPosX, ndc).x;
   distance = abs(R.x);
   }

   if ((R.x < -zNear) && (abs(R.y) < -R.x) && (abs(R.z) < -R.x)){
   ndc = (clipSpaceNegX.xy/clipSpaceNegX.w)/2.0 + 0.5;
   z_b = texture2D(shadowNegX, ndc).x;
   distance = abs(R.x);
   }

   if ((R.y > zNear) && (abs(R.x) < R.y) && (abs(R.z) < R.y)){
   ndc = (clipSpacePosY.xy/clipSpacePosY.w)/2.0 + 0.5;
   z_b = texture2D(shadowPosY, ndc).x;
   distance = abs(R.y);
   }

   if ((R.y < -zNear) && (abs(R.x) < -R.y) && (abs(R.z) < -R.y)){
   ndc = (clipSpaceNegY.xy/clipSpaceNegY.w)/2.0 + 0.5;
   z_b = texture2D(shadowNegY, ndc).x;
   distance = abs(R.y);
   }

   if ((R.z > zNear) && (abs(R.x) < R.z) && (abs(R.y) < R.z)){
   ndc = (clipSpacePosZ.xy/clipSpacePosZ.w)/2.0 + 0.5;
   z_b = texture2D(shadowPosZ, ndc).x;
   distance = abs(R.z);
   }

   if ((R.z < -zNear) && (abs(R.x) < -R.z) && (abs(R.y) < -R.z)){
   ndc = (clipSpaceNegZ.xy/clipSpaceNegZ.w)/2.0 + 0.5;
   z_b = texture2D(shadowNegZ, ndc).x;
   distance = abs(R.z);
   }

   //distance = length(r);
   float z_n = 2.0*z_b - 1.0;
   float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n*(zFar - zNear));
   //float z_e = 0.9009/z_b;
   vec3 shadow;
   
   if ((z_e + depthBias) < distance){
   shadow = vec3(0.2,0.2,0.2);
   }
   else {
   shadow = vec3(1.0,1.0,1.0);
   }

   float rsquare = pow(length(r),20); // for attenuation
   
   //diffuse:
   float brightness = clamp(dot(lightVector, normalWorld),0,1)/rsquare;
   vec3 diffuse = vec3(brightness, brightness, brightness);

   //specular:
   vec3 reflectedLightVector = -reflect(lightVector,normalWorld);
   vec3 eyeDirection = normalize(eyePosition - positionWorld);
   float s = clamp(dot(reflectedLightVector, eyeDirection),0,1);
   float ss = pow(s,200)/length(r);
   vec3 specular = vec3(ss, ss, ss) * specularity;

   vec3 light = diffuse + ambient + specular;
//daColor =  texture(shadowPosX, TexCoord);
//daColor = vec4(theColor, 1.0);
daColor = texture(ourTexture1, TexCoord) * vec4(light, alpha)* vec4(shadow,1.0);//punktweises Produkt
daColor = mix(vec4(fogColor,1.0),daColor,visibility);
//gl_FragDepth = gl_FragCoord.z;
}

