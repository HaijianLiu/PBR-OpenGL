#version 330 core

in vec2 uv;
in vec3 lightDirectionCameraspace;
// in vec3 texNormalCameraspace;

// Ouput data
out vec3 color;

uniform sampler2D texDiffuse;
uniform sampler2D texAO;
uniform sampler2D texNormal;

// uniform mat4 matrixMVP;
// uniform mat4 matrixModel;
// uniform mat4 matrixView;



void main(){

	// vec3 normal = normalize(texNormalCameraspace);
	vec3 light = normalize(lightDirectionCameraspace);
  // Output color = color of the texture at the specified UV
  // color = vec3(UV,1.0);
// texture(texBaseColor,uv).rgb

	vec3 colorDiffuse = texture(texDiffuse,uv).rgb;
	vec3 colorAO = vec3(1 - texture(texAO,uv).r);
	vec3 colorNormal = texture(texNormal,uv).rgb;
	vec3 lightColor = vec3(1);
	float cosLightAngle = clamp(dot(colorNormal,light),0,1);

  // color = texture(texBaseColor,uv).rgb - 0.1*(vec3(1.0) - texture(texAO,uv).rgb);
	// color = texture(texDiffuse,uv).rgb;
  // color = normal;
	// color = colorNormal;
	// color = texture(texNormal,uv).rgb;
	// Normal of the computed fragment, in camera space

	// color = colorDiffuse * cosLightAngle * lightColor * lightPower / (lightDistance * lightDistance);
	// color = colorDiffuse - 0.5*colorAO;
	// color = colorAO;
	// color = normal;
	color = (colorDiffuse - 0.5*colorAO) * cosLightAngle * lightColor;
	// color = cosLightAngle * vec3(1);
	// color = light;


}
