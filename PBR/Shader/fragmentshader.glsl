#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;
in vec3 l;
in vec3 e;
in float lightDistance;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texAO;
uniform sampler2D texSpec;



void main(){

	// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
	vec3 normalTangentspace = 2*texture(texNormal,uv).rgb - 1;

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(normalTangentspace);

	float cosLight  = clamp(dot(n,l),0,1);
	vec3 r = reflect(l,n);
	float cosReflect = clamp(dot(e,r),0,1);


	// Light emission properties
	// You probably want to put them as uniforms
	vec3 lightColor = vec3(1);
	float lightPower = 5.0f;


	// Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(1)-vec3(texture(texAO,uv).r);
	vec3 materialSpecularColor = vec3(texture(texSpec,uv).r);


	// color = (vec3(1)-materialSpecularColor) * materialDiffuseColor * lightColor * lightPower / lightDistance;

	color =
		// Ambient : simulates indirect lighting
		- 0.5 * materialAmbientColor +
		// Diffuse : "color" of the object
		(vec3(1)-materialSpecularColor) * materialDiffuseColor * lightColor * lightPower / lightDistance +
		// Specular : reflective highlight, like a mirror
		2.0 * materialDiffuseColor * materialSpecularColor * cosReflect * lightColor * lightPower / lightDistance;

}
