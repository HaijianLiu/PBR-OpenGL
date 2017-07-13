#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertex;
in vec2 uv;
in mat3 tbn;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texMetal;
uniform sampler2D texRough;
uniform sampler2D texAO;
uniform samplerCube texSky;

uniform mat4 matrixModel;

float PI = 3.14159265359;
float roughness = 0.5;
float specular(float cosHalfway, float roughness) {
	return pow(cosHalfway,roughness); // 1/2 2 = 1/4   1/3 2 = 1/9    1/4 2 = 1/6
}

float fresnelSchlick(float cosView, float n, float roughness) {
	float f0 = pow((1-n)/(1+n),2);
	return mix(f0+(1.0 - f0) * pow(1.0 - cosView, 5.0),1-f0,roughness);
}

vec3 fresnelSchlick(float cosView, vec3 materialDiffuseColor, vec3 metalness)
{
	vec3 f0 = vec3(0.04);
	f0 = mix(f0,materialDiffuseColor,metalness);
	return f0 + (1.0 - f0) * pow(1.0 - cosView, 5.0);
}
// vec3 f0 = vec3(0.04);
// f0 = mix(f0,albedo,metallic);

float fresnelSchlick(float cosView, float f0) {
	return f0 + (1.0 - f0) * pow(1.0 - cosView, 5.0);
}

float distributionGGX(float cosHalfway, float roughness) {
	float nom   = pow(roughness,4);
	float denom = pow(cosHalfway,2) * (nom - 1.0) + 1.0;
	denom = PI * denom * denom;

	return nom / denom;
}

float geometrySchlickGGX(float cosView, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom   = cosView;
	float denom = cosView * (1.0 - k) + k;

	return nom / denom;
}

float geometrySmith(float cosView, float cosLight, float roughness) {
	float ggx1  = geometrySchlickGGX(cosLight, roughness);
	float ggx2  = geometrySchlickGGX(cosView, roughness);

	return ggx1 * ggx2;
}

// -----------------------------------------------------------------------------

vec3 exposureToneMapping(vec3 color, float exposure) {
	return color = vec3(1.0) - exp(-color * exposure);
}

vec3 reinhardToneMapping(vec3 color) {
	// reinhard tone mapping
	return color = color / (color + vec3(1.0));
}

vec3 richardToneMapping(vec3 color) {
	// optimized formula by Jim Hejl and Richard Burgess-Dawson
	vec3 x = max(vec3(0),color - vec3(0.004));
	return color = (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + vec3(0.06));
}

vec3 uncharted2ToneMaping(vec3 color) {
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	return ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
}

vec3 gammaCorrection(vec3 color, float gamma) {
	// gamma correction
	return color = pow(color,vec3(1.0 / gamma));
}

float fresnel_dielectric_cos(float cosi, float eta)
{
	/* compute fresnel reflectance without explicitly computing
	 * the refracted direction */
	float c = abs(cosi);
	float g = eta * eta - 1 + c * c;
	float result;

	if (g > 0) {
		g = sqrt(g);
		float A = (g - c) / (g + c);
		float B = (c * (g + c) - 1) / (c * (g - c) + 1);
		result = 0.5 * A * A * (1 + B * B);
	}
	else
		result = 1.0;  /* TIR (no refracted component) */

	return result;
}

void main(){

	// // Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(1)-vec3(texture(texAO,uv).r);
	vec3 materialMetallicColor = vec3(texture(texMetal,uv).r);
	vec3 materialRoughnessColor = vec3(texture(texRough,uv).r);
	vec3 materialNormalColor = texture(texNormal,uv).rgb;
	materialNormalColor = normalize(materialNormalColor * 2.0 - 1.0);

	vec3 vertexWorldspace = (matrixModel * vec4(vertex,1)).xyz;
	vec3 normalWorldspace = normalize(tbn * materialNormalColor);

	vec3 hdrColor = vec3(3,2.7,2.5);
	vec3 lightPositionWorldspace = vec3(20,40,20);
	vec3 eyeWorldspace = vec3(0,16,20);

	vec3 eyeDirectionWorldspace = normalize(eyeWorldspace - vertexWorldspace);
	vec3 lightDirectionWorldspace = normalize(lightPositionWorldspace - vertexWorldspace);
	vec3 halfwayDirectionWorldspace = normalize(lightDirectionWorldspace + eyeDirectionWorldspace);

	float cosView    = max(dot(normalWorldspace,eyeDirectionWorldspace),0);
	float cosLight   = max(dot(normalWorldspace,lightDirectionWorldspace),0);
	float cosHalfway = max(dot(normalWorldspace,halfwayDirectionWorldspace),0);


	color =

	hdrColor *
	fresnelSchlick(cosView,materialDiffuseColor,materialMetallicColor) * (0.04 + specular(cosHalfway,10*(1-materialRoughnessColor.x)))
	+
	materialDiffuseColor *
	(
		1 -
		fresnelSchlick(cosView,materialDiffuseColor,materialMetallicColor) * specular(cosHalfway,10*(1-materialRoughnessColor.x))
	)
	- 0.5 * materialAmbientColor;

	color = max(vec3(0),color);

	// color = reflect(eyeDirectionWorldspace,normalWorldspace);
	// color = texture(texSky,uv).rgb;

	// (1-materialMetallicColor) *
	// (
	// 	( 1-fresnelSchlick(cosView,0.04) * specular(cosHalfway,10*(1-materialRoughnessColor.x)) ) * materialDiffuseColor
	// 	+ fresnelSchlick(cosView,0.04) * specular(cosHalfway,10*(1-materialRoughnessColor.x))     * hdrColor
	// )
	// +
	// materialMetallicColor *
	// (
	// 	specular(cosHalfway,10*(1-materialRoughnessColor.x)) * hdrColor
	// 	+ ( 1 - specular(cosHalfway,10*(1-materialRoughnessColor.x)) ) * fresnelSchlick(cosView,materialDiffuseColor,materialMetallicColor) * hdrColor
	// )
	// - 0.9 * materialAmbientColor;

	// (1-materialMetallicColor) * fresnelSchlick(cosView,0.04) * specular(cosHalfway,0.5) * hdrColor ;
	// fresnelSchlick(cosView,materialDiffuseColor,materialMetallicColor)*specular(cosHalfway,materialRoughnessColor.x*1);
	// (1 - materialMetallicColor) * ( fresnelSchlick(cosView,0.04) * specular(cosHalfway,materialRoughnessColor.x) * hdrColor + (1 - fresnelSchlick(cosView,0.04) * specular(cosHalfway,materialRoughnessColor.x) ) * materialDiffuseColor )
	// + materialMetallicColor * ( fresnelSchlick(cosView,0.8) *  specular(cosHalfway,materialRoughnessColor.x) * materialDiffuseColor)
	// (1 - materialMetallicColor) * (fresnelSchlick(cosView,0.04) * hdrColor + (1 - fresnelSchlick(cosView,0.04)) * materialDiffuseColor);

	// (1 - materialMetallicColor) * (fresnelSchlick(cosView,2.8735,0.7) * materialDiffuseColor + lightColor * 0.05/0.7 * specular(cosHalfway,10/0.7))
	// + materialMetallicColor * (fresnelSchlick(cosView,1.6232,0.15) * 3 * materialDiffuseColor + lightColor * 0.05/0.15 * specular(cosHalfway,10/0.15))
	// - 0.7 * materialAmbientColor;
	color *= 8;


	// float metallic = 0;
	// float roughness = 0.03;
	// float albedo = 10;
	// float f0 = mix(0.04,albedo,roughness);
	// // cook-torrance brdf
	// float NDF = distributionGGX(cosHalfway,roughness);
	// float G   = geometrySmith  (cosView,cosLight,roughness);
	// float F   = fresnelSchlick (cosView,f0);
	//
	// float kS = F;
	// float kD = 1 - kS;
	//       kD *= 1.0 - metallic;
	//
	// float nominator   = NDF * G * F;
	// float denominator = 4 * cosView * cosLight + 0.001;
	// float specular    = nominator / denominator;
	//
	// // add to outgoing radiance Lo
	// float lo = (kD * albedo / PI + specular) * cosLight;
	//
	// float ambient = 0.03 * albedo;
	//
	// color = materialDiffuseColor * (ambient + lo);


	// color = exposureToneMapping(color,1); // Light intense
	// color = lightColor * geometrySmith(cosView,cosLight,0.9);

	color = uncharted2ToneMaping(color);
	color = color / uncharted2ToneMaping(vec3(11.2));


}
