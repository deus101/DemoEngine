#version 330
out float FragColor;
//in vec2 TexCoords;


//with depth
uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gTexNoise;

uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

uniform vec3 gEyeWorldPos;
uniform vec2 gScreenSize;

int MAX_KERNELS = 64;
float radius = 1.0;

uniform vec3 gKernel[64];


const vec2 noiseScale = vec2(1280.0f/4.0f, 720.0f/4.0f); 


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}


void main()
{
	vec2 TexCoords = CalcTexCoord();
    // Get input for SSAO algorithm

    vec3 fragPos = (gView * vec4( texture(gPositionMap, TexCoords).xyz, 1.0f)).xyz;


    vec3 normal = texture(gTexNoise, TexCoords).rgb;
    vec3 randomVec = (gView * vec4(texture(gNormalMap, TexCoords * noiseScale).xyz,0.0f)).xyz;
	
    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    
	
	// Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < MAX_KERNELS; ++i)
    {
        // get sample position
        vec3 sample = TBN * gKernel[i]; // From tangent to view-space
        sample = fragPos + sample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        
		
		offset = gProjection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = -texture(gPositionMap, offset.xy).w; // Get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / MAX_KERNELS);
	FragColor = occlusion;
}