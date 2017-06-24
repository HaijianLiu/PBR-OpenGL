#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertex;
in vec2 uv;
in vec3 normal;
in mat3 tbn;

out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texAO;
uniform sampler2D texSpec;
uniform mat4 matrixModel;


float specular(float cosReflect, float shininess) {
	return pow(cosReflect,shininess);
}

float fresnelSchlick(float cosTheta, float n, float roughness) {
	float f0 = pow((1-n)/(1+n),2);
	return mix(f0+(1.0 - f0) * pow(1.0 - cosTheta, 5.0),1-f0,roughness);
}

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


void main(){

	// // Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(1)-vec3(texture(texAO,uv).r);
	vec3 materialSpecularColor = vec3(texture(texSpec,uv).r);
	vec3 materialNormalColor = texture(texNormal,uv).rgb;
	materialNormalColor = normalize(materialNormalColor * 2.0 - 1.0);

	vec3 vertexWorldspace = (matrixModel * vec4(vertex,1)).xyz;
	vec3 normalWorldspace = normalize(tbn * materialNormalColor);

	vec3 lightColor = vec3(1);
	vec3 lightPositionWorldspace = vec3(20,40,20);
	vec3 eyeWorldspace = vec3(0,16,20);

	vec3 eyeDirectionWorldspace = normalize(eyeWorldspace - vertexWorldspace);
	vec3 lightDirectionWorldspace = normalize(lightPositionWorldspace - vertexWorldspace);
	vec3 halfwayDirectionWorldspace = normalize(lightDirectionWorldspace + eyeDirectionWorldspace);

	float cosTheta = dot(eyeDirectionWorldspace,normalWorldspace);
	float cosReflect = max(dot(normalWorldspace,halfwayDirectionWorldspace),0.0);

	color =
	(1 - materialSpecularColor) * (fresnelSchlick(cosTheta,1.0,0.8) * materialDiffuseColor + lightColor * 0.1/0.8 * specular(cosReflect,10/0.8))
	+ materialSpecularColor * (fresnelSchlick(cosTheta,1.45,0.15) * 3 * materialDiffuseColor + lightColor * 0.1/0.15 * specular(cosReflect,10/0.15))
	- 0.6 * materialAmbientColor;
	color *= 7;

	// color = exposureToneMapping(color,1); // Light intense

	color = uncharted2ToneMaping(color);
	color = color / uncharted2ToneMaping(vec3(11.2));

}
