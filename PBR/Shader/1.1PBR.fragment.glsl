#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertex;
in vec2 uv;
in mat3 tbn;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texRough;
uniform sampler2D texAO;
uniform samplerCube texSky;

uniform mat4 matrixModel;

uniform vec3 eyeWorldspace;


const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float distributionGGX(float cosHalfway, float roughness) {
	float denom = (cosHalfway * cosHalfway * (roughness * roughness - 1.0) + 1.0);
	denom = PI * denom * denom;
	return roughness * roughness / denom;
}
// ----------------------------------------------------------------------------
float geometrySchlickGGX(float cosViewAndLight, float roughness) {
float r = (roughness + 1.0);
float k = (r*r) / 8.0;

float nom   = cosViewAndLight;
float denom = cosViewAndLight * (1.0 - k) + k;

return nom / denom;
}
// ----------------------------------------------------------------------------
float geometrySmith(float cosLight, float cosView, float roughness) {
float ggx2 = geometrySchlickGGX(cosView, roughness);
float ggx1 = geometrySchlickGGX(cosLight, roughness);

return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosView, vec3 F0) {
return F0 + (1.0 - F0) * pow(1.0 - cosView, 5.0);
}
// ----------------------------------------------------------------------------
vec3 uncharted2ToneMaping(vec3 color) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
}

void main()
{
	// Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(texture(texAO,uv).r);
	vec3 materialRoughnessColor = vec3(texture(texRough,uv).r);
	vec3 materialNormalColor = texture(texNormal,uv).rgb;
	materialNormalColor = normalize(materialNormalColor * 2.0 - 1.0);

	vec3 vertexWorldspace = (matrixModel * vec4(vertex,1)).xyz;
	vec3 normalWorldspace = normalize(tbn * materialNormalColor);

	vec3 hdrColor = vec3(3,2.7,2.5);
	vec3 lightPositionWorldspace = vec3(20,40,20);

	vec3 eyeDirectionWorldspace = normalize(eyeWorldspace - vertexWorldspace);
	vec3 lightDirectionWorldspace = normalize(lightPositionWorldspace - vertexWorldspace);
	vec3 halfwayDirectionWorldspace = normalize(lightDirectionWorldspace + eyeDirectionWorldspace);

	float cosView    = max(dot(normalWorldspace,eyeDirectionWorldspace),0);
	float cosHalfway = max(dot(normalWorldspace,halfwayDirectionWorldspace),0);
	float cosLight   = max(dot(normalWorldspace,lightPositionWorldspace),0);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	vec3 F0 = vec3(0.04);
	// F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);

	// calculate per-light radiance
	// float distance = length(lightPositions[i] - WorldPos);
	// float attenuation = 1.0 / (distance * distance);
	// vec3 radiance = lightColors[i] * attenuation;

	// Cook-Torrance BRDF
	float NDF = distributionGGX(cosHalfway, materialRoughnessColor.r);
	float G   = geometrySmith(cosLight, cosView, materialRoughnessColor.r);
	vec3  F   = fresnelSchlick(cosView, F0);

	vec3 nominator    = NDF * G * F;
	float denominator = 4 * cosView * cosLight + 0.001; // 0.001 to prevent divide by zero.
	vec3 specular     = nominator / denominator;

	// kS is equal to Fresnel
	vec3 kS = F;
	// for energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kD) should equal 1.0 - kS.
	vec3 kD = vec3(1.0) - kS;
	// multiply kD by the inverse metalness such that only non-metals
	// have diffuse lighting, or a linear blend if partly metal (pure metals
	// have no diffuse light).
	// kD *= 1.0 - metallic;

	// add to outgoing radiance Lo
	Lo += (kD * materialDiffuseColor / PI + specular) * cosLight / 20;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

	// ambient lighting (note that the next IBL tutorial will replace
	// this ambient lighting with environment lighting).
	vec3 ambient = vec3(0.03) * materialDiffuseColor;

	color = ambient + Lo;


	// // tonemapping and gammer correction
	// color = uncharted2ToneMaping(color);
	// color = color / uncharted2ToneMaping(vec3(11.2));

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));
}
