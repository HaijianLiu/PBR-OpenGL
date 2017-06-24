#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertex;
in vec2 uv;
in vec3 normal;
// in vec3 l;
// in vec3 e;
// in float lightDistance;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texAO;
uniform sampler2D texSpec;
uniform mat3 matrixModelView3x3;
uniform mat4 matrixView;
uniform mat4 matrixProject;
uniform mat4 matrixMVP;
// uniform mat4 matrixModel;

// vec3 mixColor(vec3 color1, vec3 color2, float percent){
// 	return percent * color1 + (1.0 - percent) * color2;
// }
//
// float fresnel(float ior, float roughness){
//
// 	ior = sin1/sin2
// 	return opacity - pow(dot(normalCameraspace,vec3(0,0,1)),range);
// }

// float kS = calculateSpecularComponent(...); // reflection/specular fraction
// float kD = 1.0 - kS;                        // refraction/diffuse  fraction


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


	vec3 lightPositionWorldspace = vec3(0,50,200);

	vec3 lightPositionCameraspace = (matrixProject * matrixView * vec4(lightPositionWorldspace,1)).xyz;

	vec3 vertexCameraspace = (matrixMVP * vec4(vertex,1)).xyz;

	vec3 lightDirectionCameraspace = normalize(vertexCameraspace - lightPositionCameraspace);

	vec3 normalCameraspace = normalize(matrixModelView3x3 * normal);

	float cosTheta = dot(lightDirectionCameraspace,normalCameraspace);


	// float fresnel = fresnelSchlick(cosTheta,f0);

	color = (1 - materialSpecularColor) * fresnelSchlick(cosTheta,1.0,0.4) * materialDiffuseColor + materialSpecularColor * fresnelSchlick(cosTheta,1.45,0.1) * materialDiffuseColor - 0.3 * materialAmbientColor;
	color *= 8;
	// color = exposureToneMapping(color,1); // Light intense

	color = uncharted2ToneMaping(color);
	color = color / uncharted2ToneMaping(vec3(11.2));
	// color = gammaCorrection(color,2.2);


	// fresnel * materialSpecularColor + materialDiffuseColor;
	// fresnel * materialDiffuseColor;
	// vec3(cosTheta);
	// cosTheta * materialDiffuseColor - 0.3 * materialAmbientColor;
	// (1 - materialSpecularColor) * mixColor(3 * materialDiffuseColor * ambientShader, 3 * materialDiffuseColor * fresnel(1,0.5,normalCameraspace),0.4) + materialSpecularColor *  1.5 * mixColor(materialDiffuseColor, materialSpecularColor * fresnel(1,0.5,normalCameraspace),0.1) - 0.7 * materialAmbientColor;
	// vec3(pow(dot(normalize(vertexCameraspace),normalize(normalCameraspace)),2));


	// color =
	// 	// Ambient : simulates indirect lighting
	// 	- 0.5 * materialAmbientColor +
	// 	// Diffuse : "color" of the object
	// 	(vec3(1)-materialSpecularColor) * materialDiffuseColor * lightColor * lightPower / lightDistance +
	// 	// Specular : reflective highlight, like a mirror
	// 	2.0 * materialDiffuseColor * materialSpecularColor * cosReflect * lightColor * lightPower / lightDistance;

}
