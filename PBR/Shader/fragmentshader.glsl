#version 330 core

in vec2 UV;

// Ouput data
out vec3 color;

uniform sampler2D myTextureSampler;


void main(){
  // Output color = color of the texture at the specified UV
  // color = vec3(UV,1.0);
  color = texture(myTextureSampler,UV).rgb;

}
