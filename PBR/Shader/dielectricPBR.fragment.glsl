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


float specular(float cosHalfway, float roughness) {
	return pow(cosHalfway,roughness);
}

float fresnelSchlick(float cosView) {
	float f0 = 0.04;
	return f0 + (1.0 - f0) * pow(1.0 - cosView, 5.0);
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

	// Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(1)-vec3(texture(texAO,uv).r);
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
	float cosHalfway = max(dot(normalWorldspace,halfwayDirectionWorldspace),0);


	// basic color
	color =
	// reflect color
	hdrColor *
	fresnelSchlick(cosView) * specular(cosHalfway,materialRoughnessColor.x)
	+
	// diffuse color
	materialDiffuseColor *
	( 1 - fresnelSchlick(cosView) * specular(cosHalfway,materialRoughnessColor.x) )
	// amibent color
	- 0.3 * materialAmbientColor;
	// clamping min color to zero
	color = max(vec3(0),color);

	// exposure
	color *= 5;

	// tonemapping and gammer correction
	color = uncharted2ToneMaping(color);
	color = color / uncharted2ToneMaping(vec3(11.2));

}
