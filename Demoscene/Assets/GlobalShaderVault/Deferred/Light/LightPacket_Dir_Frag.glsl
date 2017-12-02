#version 330
//Directional Light
struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

//here I will add glsl code automatically


uniform sampler2D baseMaterialMap;
uniform int baseMaterialCount;

//uniform sampler2D gColorMap;

uniform sampler2D gbPositionMap;
uniform sampler2D gbAbedoMap;
uniform sampler2D gbNormalMap;
uniform sampler2D gbUvMap;
uniform sampler2D aobAoMap;

//uniform sampler2D baseMaterialMap;


uniform mat4 commonProjectionMatrix;
uniform mat4 commonViewMatrix;	
uniform mat4 commonWorldMatrix;

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLight;
uniform SpotLight gSpotLight;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity = 0.9f;
uniform float gSpecularPower = 8.0f;
uniform int gLightType;
uniform vec2 gScreenSize;


vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / gScreenSize;
}

//if we run a effect lookup first we can anticipate what paramaters we require
//void LookUpMaterial(int ID, out vec3 MatDiffuse,out vec3 MatSpecular,out float MatEmmi)

#include MaterialFunction



vec4 CalcLightInternal(BaseLight Light,
	vec3 LightDirection,
	float ID,
	vec3 WorldPos,
	float Depth,
	vec3 Normal,
	float AO)
{

	//(Light.AmbientIntensity * AO));


	vec4 DiffuseColor = vec4(0, 0, 0, 0);
	vec4 SpecularColor = vec4(0, 0, 0, 0);
	vec3 DiffuseColor1 = vec3(0, 0, 0);
	vec3 SpecularColor1 = vec3(0, 0, 0);


	LookUpMaterial(ID, DiffuseColor, SpecularColor);
	

	vec2 tc = CalcTexCoord();
	DiffuseColor.xyz = texture(gbAbedoMap, tc).xyz;

	vec3 AmbientColor = vec3(Light.AmbientIntensity * DiffuseColor.xyz *  AO);
	//AmbientColor *= OcculsionFactor;
	//float DiffuseFactor = max(dot(Normal, LightDirection),0.0);
	float DiffuseFactor = dot(Normal, LightDirection);


	if (DiffuseFactor > 0.0) {
		DiffuseColor1 = Light.DiffuseIntensity * (DiffuseColor.xyz * Light.Color);
		//* Light.DiffuseIntensity * 
		//vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
		//vec3 VertexToEye = normalize(WorldPos - gEyeWorldPos);
		//vec3 VertexToEye = normalize(-WorldPos);
		//vec3 FragUV = vec3(vec2(gl_FragCoord.xy/gScreenSize ), 0.0);
		vec2 madd = vec2(0.5, 0.5);


		vec3 VertexToEye = normalize((-WorldPos) + ((vec3(vec2(gl_FragCoord.xy / gScreenSize)*madd + madd, 0.0) + gEyeWorldPos)));
		vec3 halfwayDir = normalize(LightDirection + VertexToEye);


		//mat3 viewNormalMat = transpose(mat3(commonViewMatrix));
		//vec3 ViewNorm = viewNormalMat * Normal;
		//vec3 LightReflect = normalize(reflect( Normal,LightDirection));
		//vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		//float SpecularFactor = dot(VertexToEye,LightReflect );
		//float SpecularFactor = dot(LightReflect,VertexToEye );
		float SpecularFactor = dot(Normal, halfwayDir);

		SpecularFactor = pow(SpecularFactor, gSpecularPower);
		if (SpecularFactor > 0.0) {
			//SpecularColor1 = Light.Color * gMatSpecularIntensity;
			SpecularColor1 = Light.Color * gMatSpecularIntensity;
			SpecularColor1 *= SpecularColor.xyz  *SpecularFactor;
			//gMatSpecularIntensity * SpecularColor
		}
	}

	return vec4(AmbientColor += (DiffuseColor1 + SpecularColor1), 1.0);
	//return vec4(AmbientColor * (DiffuseColor1 + SpecularColor1), 1.0 	);
	//return vec4(SpecularColor1, 1.0 	);
	//return (AmbientColor + SpecularColor );
}

vec4 CalcDirectionalLight(float ID, vec3 WorldPos, float Depth, vec3 Normal, float AO)
{
	return CalcLightInternal(gDirectionalLight.Base,
		gDirectionalLight.Direction,
		ID,
		WorldPos,
		Depth,
		Normal,
		AO);
}

vec4 CalcPointLight(float ID, vec3 WorldPos, float Depth, vec3 Normal, float AO)
{
	vec3 LightDirection = WorldPos - gPointLight.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, ID, WorldPos, Depth, Normal, AO);

	float Attenuation = (gPointLight.Atten.Constant +
		gPointLight.Atten.Linear * Distance +
		gPointLight.Atten.Exp * (Distance * Distance));

	//float Attenuation =  gPointLight.Atten.Constant +
	//                     gPointLight.Atten.Linear * Distance +
	//                     gPointLight.Atten.Exp * Distance * Distance;

	Attenuation = max(1.0, Attenuation);

	return Color / Attenuation;
}




out vec4 FragColor;

void main()
{
	//mat3 viewNormal = transpose(inverse(mat3(commonViewMatrix)));
	//mat3 viewNormal = transpose(mat3(commonViewMatrix));
	//int draw_mode = 0;
	//float debug = 1.0f;
	//vec4 Debugger = vec4(0, 0, 0, 0);

	vec2 TexCoord = CalcTexCoord();
	vec3 WorldPos = texture(gbPositionMap, TexCoord).xyz;
	float Depth = texture(gbPositionMap, TexCoord).a;
	//material id
	vec3 Color = texture(gbAbedoMap, TexCoord).xyz;

	float MatId = texture(gbUvMap, TexCoord).z;




	vec4 Diff;
	vec4 Spec;

	LookUpMaterial(MatId, Diff, Spec);


	vec3 Normal = texture(gbNormalMap, TexCoord).xyz;
	vec2 Uv = texture(gbUvMap, TexCoord).xy;
	
	float AmbientOcculsion = texture(aobAoMap, TexCoord).r;


	vec3 satan = Normal;

	//FragColor = vec4(Color, 1.0) *  CalcDirectionalLight(MatId,WorldPos, Depth,satan,AmbientOcculsion);
	FragColor = CalcDirectionalLight(MatId, WorldPos, Depth, satan, AmbientOcculsion);


}