#version 430

out vec4 daColor;

in vec3 positionWorld;
in vec3 normalWorld;

in vec3 theColor;

in vec4 clipSpace;

in vec2 textureCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D dudvMap;
uniform float texMove;
uniform sampler2D normalMap;

float waveHeight=0.002;

uniform vec3 specularity;
uniform vec3 eyePosition;
uniform vec3 lightPosition;

void main()
{

//projected window viewport x-y-coords:
vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;// (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

//reflection texture coords:
vec2 reflectTexCoord = vec2(ndc.x,1.0-ndc.y);

//refraction texture coords:
vec2 refractTexCoord = vec2(ndc.x,ndc.y);

vec2 distortion = (texture(dudvMap, vec2(textureCoord.x + texMove, textureCoord.y - texMove)).rg*2.0 - 1.0)*waveHeight;

reflectTexCoord+=distortion;
refractTexCoord+=distortion;

 vec4 normalColor = texture(normalMap, vec2(textureCoord.x + texMove, textureCoord.y - texMove));
 vec3 normal = vec3(normalColor.r*2.0-1.0,normalColor.b,normalColor.g*2.0-1.0);
 normal = normalize(normal);

 vec3 r = lightPosition - positionWorld;
 vec3 lightVector = normalize(r);

 //specular light:
 vec3 reflectedLightVector = -reflect(lightVector,normal);
 vec3 eyeDirection = normalize(eyePosition - positionWorld);
 float s = clamp(dot(reflectedLightVector, eyeDirection),0,1);
 float ss = pow(s,500)/length(r);
 vec3 specular = vec3(ss, ss, ss) * specularity;

vec3 light = specular;
vec4 reflectColor = texture(ourTexture1, reflectTexCoord)+ vec4(light,0.0);;
vec4 refractColor = texture(ourTexture2, refractTexCoord);

//daColor = vec4(theColor, 1.0);
//daColor = texture(ourTexture1, reflectTexCoord);
//daColor = normalColor;
daColor = mix(reflectColor,refractColor,eyeDirection.y);
gl_FragDepth=gl_FragCoord.z;
}