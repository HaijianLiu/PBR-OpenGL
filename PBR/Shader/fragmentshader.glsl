#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;
in vec3 vertex;
in vec3 normal;
in float cosLight;
in float cosReflect;
in float lightDistance;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texDiffuse;
// uniform sampler2D texNormal;



void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 lightColor = vec3(1);
	float lightPower = 50.0f;


	// Material properties
	vec3 materialDiffuseColor = texture(texDiffuse,uv).rgb;
	vec3 materialAmbientColor = vec3(0.5) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.3);



	color =
		// Ambient : simulates indirect lighting
		materialAmbientColor +
		// Diffuse : "color" of the object
		materialDiffuseColor * lightColor * lightPower * cosLight / (lightDistance * lightDistance) +
		// Specular : reflective highlight, like a mirror
		materialSpecularColor * lightColor * lightPower * pow(cosReflect,5) / (lightDistance * lightDistance);

}
