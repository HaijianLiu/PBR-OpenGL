#version 330 core

in vec2 uv;

// Ouput data
out vec3 color;

uniform sampler2D texDiffuse;
uniform sampler2D texAO;


void main(){
  // Output color = color of the texture at the specified UV
  // color = vec3(UV,1.0);
// texture(texBaseColor,uv).rgb
	vec3 colorDiffuse = texture(texDiffuse,uv).rgb;
	vec3 colorAO = vec3(1 - texture(texAO,uv).r);
  // color = texture(texBaseColor,uv).rgb - 0.1*(vec3(1.0) - texture(texAO,uv).rgb);
	// color = texture(texDiffuse,uv).rgb;
  // color = normal;
	color = colorDiffuse - 0.5*colorAO;
	// color = colorAO;

}
