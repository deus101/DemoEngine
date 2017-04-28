#version 330
                                                                        
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
in vec4 ModelData;  
flat in int InstanceID0; 


layout (location = 0) out vec4 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    
							
//uniform sampler2D gColorMap;     

uniform vec3 mDiffuseCol;             
uniform float mSpecularInt;
uniform float mSpecularPow;  

const float NEAR = 0.1f;
const float FAR = 50.0f;

float LinearDepth(float depth)
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}
 											
void main()									
{	//With depth										
	//WorldPosOut.xyz     = WorldPos0;
	//WorldPosOut.w = LinearDepth(gl_FragCoord.z);

	WorldPosOut.xyz = WorldPos0;

	WorldPosOut.w = LinearDepth(gl_FragCoord.z);
	//WorldPosOut.w = gl_FragCoord.z;


	//DiffuseOut = vec3(InstanceID0 * 20.0, 0.0, 0.0);	
	//DiffuseOut = vec3(   ModelData.x  , ModelData.z, 0.0);
	DiffuseOut = vec3(  (ModelData.x ) / 10 , 0 ,  (ModelData.z ));	
	//if( textureSize( gColorMap, 0).x > 0)
	//{					
	//DiffuseOut      = vec3(TexCoord0,0);
	//DiffuseOut      = texture(gColorMap, TexCoord0).xyz;	
	//}
	//else
	//{
	//DiffuseOut      = mDiffuseCol;	
	//}
	
	NormalOut       = normalize(Normal0);					
	//TexCoordOut     = vec3(TexCoord0, 0.0);	
	//cheap hack	
	TexCoordOut     = vec3(TexCoord0,floor(ModelData.x+0.1));		
	//TexCoordOut     = vec3(TexCoord0,2);	
	//So lightpass gets wrong samnpler but let me just try to see if it works
}
