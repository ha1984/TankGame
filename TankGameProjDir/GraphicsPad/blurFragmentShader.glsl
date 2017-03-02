#version 430

out vec4 daColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float depth;

in vec2 horizBlurCoord[11];
in vec2 vertBlurCoord[11];

in vec4 clipSpace;

void main()
{

// blur texture coords:

// horizontal Blur: 
vec4 horizColor = vec4(0.0,0.0,0.0,0.0);
horizColor += texture(ourTexture1, horizBlurCoord[0])*0.0093;
horizColor += texture(ourTexture1, horizBlurCoord[1])*0.028002;
horizColor += texture(ourTexture1, horizBlurCoord[2])*0.065984;
horizColor += texture(ourTexture1, horizBlurCoord[3])*0.121703;
horizColor += texture(ourTexture1, horizBlurCoord[4])*0.175713;
horizColor += texture(ourTexture1, horizBlurCoord[5])*0.198596;
horizColor += texture(ourTexture1, horizBlurCoord[6])*0.175713;
horizColor += texture(ourTexture1, horizBlurCoord[7])*0.121703;
horizColor += texture(ourTexture1, horizBlurCoord[8])*0.065984;
horizColor += texture(ourTexture1, horizBlurCoord[9])*0.028002;
horizColor += texture(ourTexture1, horizBlurCoord[10])*0.0093;

// vertical Blur: 
vec4 vertColor = vec4(0.0,0.0,0.0,0.0);
vertColor += texture(ourTexture1, vertBlurCoord[0])*0.0093;
vertColor += texture(ourTexture1, vertBlurCoord[1])*0.028002;
vertColor += texture(ourTexture1, vertBlurCoord[2])*0.065984;
vertColor += texture(ourTexture1, vertBlurCoord[3])*0.121703;
vertColor += texture(ourTexture1, vertBlurCoord[4])*0.175713;
vertColor += texture(ourTexture1, vertBlurCoord[5])*0.198596;
vertColor += texture(ourTexture1, vertBlurCoord[6])*0.175713;
vertColor += texture(ourTexture1, vertBlurCoord[7])*0.121703;
vertColor += texture(ourTexture1, vertBlurCoord[8])*0.065984;
vertColor += texture(ourTexture1, vertBlurCoord[9])*0.028002;
vertColor += texture(ourTexture1, vertBlurCoord[10])*0.0093;

// horizontal Blur: 
//vec4 horizColor = vec4(0.0,0.0,0.0,0.0);
//for (int i=0;i<11;i++){
//horizColor += texture(ourTexture1, horizBlurCoord[i]);
//}

// vertical Blur: 
//vec4 vertColor = vec4(0.0,0.0,0.0,0.0);
//for (int i=0;i<11;i++){
//vertColor += texture(ourTexture1, vertBlurCoord[i]);
//}


vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

//daColor = vec4(theColor, 1.0);
//daColor =  texture(ourTexture1,ndc);
//daColor = horizColor;
vec4 color = mix(horizColor,vertColor,0.5);
float brightness = color.r + color.g + color.b;
daColor = color * brightness * brightness;
if (depth > 0.0){
gl_FragDepth = texture(ourTexture2,ndc).r;
}
}