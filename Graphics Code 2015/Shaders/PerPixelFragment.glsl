#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
vec4 colour;
vec2 texCoord;
vec3 normal;
vec3 worldPos;
} IN;

out vec4 fragColour;

void main ( void ) {

vec4 diffuse = texture(diffuseTex1 , IN.texCoord);

if(IN.worldPos.y < 325) 
{
 diffuse = texture(diffuseTex , IN.texCoord) * (1 - (IN.worldPos.y / 325))  + texture (diffuseTex1 , IN.texCoord) *IN.worldPos.y / 325; // for smooth transition between 2 textures
}

vec3 incident = normalize ( lightPos - IN . worldPos );			//Calculation of normal surface
float lambert = max(0.0 , dot ( incident , IN . normal ));		//Cosine of the two normalised direction vectors

float dist = length( lightPos - IN . worldPos );			//Calculate how far away a surface is from a light source
float atten = 1.0 - clamp (dist / lightRadius / 10.0, 0.0 , 1.0);	//linear attenuation

vec3 viewDir = normalize ( cameraPos - IN . worldPos );			//Camera position
vec3 halfDir = normalize ( incident + viewDir );			//Half-angle between camera position and incident
 
float rFactor = max (50.0 , dot ( halfDir , IN.normal ));		//Specularity Value
float sFactor = pow ( rFactor , 0.03 );

vec3 colour = ( diffuse . rgb * lightColour . rgb);
colour += ( lightColour.rgb * sFactor ) * 0.33;
fragColour = vec4 ( colour * atten * lambert , diffuse.a );
fragColour.rgb += ( diffuse . rgb * lightColour.rgb ) * 0.1;
}