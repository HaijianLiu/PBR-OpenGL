#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D pass[7];
uniform vec3 cameraPos;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];


uniform sampler2D texNoise;
uniform vec3 samples[4];
// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 4;
float radius = 5.0;
float bias = 0.025;
// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1600.0/4.0, 1200.0/4.0);
uniform mat4 projection;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float ambientOcclusion();
vec3 kernelBlur(sampler2D tex);


void main() {
	vec3 albedo   = texture(pass[0], TexCoords).rgb;
	vec3 N        = texture(pass[1], TexCoords).rgb;
	vec3 mra      = texture(pass[2], TexCoords).rgb;
	vec3 worldPos = texture(pass[3], TexCoords).rgb;
	vec3 alpha    = texture(pass[4], TexCoords).rgb;

	vec3 V = normalize(cameraPos - worldPos);
	vec3 R = reflect(-V, N);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, vec3(mra.r));

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) {
		// calculate per-light radiance
		vec3 L = normalize(lightPositions[i] - worldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - worldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, mra.g);
		float G   = GeometrySmith(N, V, L, mra.g);
		vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator    = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
		vec3 specular = nominator / denominator;

		 // kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - mra.r;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	// ambient lighting (we now use IBL as the ambient term)
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, mra.g);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - mra.r;

	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse      = irradiance * albedo;

	// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(prefilterMap, R,  mra.g * MAX_REFLECTION_LOD).rgb;
	vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), mra.g)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

	vec3 ambient = (kD * diffuse + (mra.r + kS) * specular) * mra.b;

	vec3 color = ambient + Lo - 0.05 * (1.0 - mra.b);
	color = max(color,vec3(0));

	// float ssao = ambientOcclusion();

	color *= 4;

	// color *= ssao;

	// // HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));


	// vec3 final = albedo * alpha + color;

	FragColor = vec4(mix(albedo, color, alpha), 1.0);
}






float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
		// k direct
    // float r = (roughness + 1.0);
    // float k = (r * r) / 8.0;
		// k IBL
		float r = roughness;
    float k = (r * r) / 2.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 kernelBlur(sampler2D tex) {
	vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
	vec3 result = vec3(0.0);
	for (int x = -2; x < 2; ++x) {
		for (int y = -2; y < 2; ++y) {
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(tex, TexCoords + offset).rgb;
		}
	}
	return result / (4.0 * 4.0);
}

float ambientOcclusion() {
	// get input for SSAO algorithm
	vec3 fragPos = texture(pass[5], TexCoords).rgb;
	vec3 normal = texture(pass[6], TexCoords).rgb;
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).rgb);
	// create TBN change-of-basis matrix: from tangent-space to view-space
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// iterate over the sample kernel and calculate occlusion factor
	float occlusion = 0.0;

	for(int i = 0; i < kernelSize; ++i) {
		// get sample position
		vec3 sampleTBN = TBN * samples[i]; // from tangent to view-space
		sampleTBN = fragPos + sampleTBN * radius;

		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(sampleTBN, 1.0);
		offset = projection * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		// get sample depth
		float sampleDepth = texture(pass[5], offset.xy).z; // get depth value of kernel sample

		// range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sampleTBN.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	return 1.0 - (occlusion / kernelSize);
}
