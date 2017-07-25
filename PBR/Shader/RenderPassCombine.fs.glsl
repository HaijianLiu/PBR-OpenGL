#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D pass0;
uniform sampler2D pass1;
// uniform bool bloom;
// uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(pass0, TexCoords).rgb;
    vec3 bloomColor = texture(pass1, TexCoords).rgb;
    // if(bloom)
        // hdrColor += bloomColor; // additive blending
    // tone mapping
    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

		vec3 color = hdrColor;
    // also gamma correct while we're at it
    // result = pow(result, vec3(1.0 / gamma));

		// HDR tonemapping
		color = color / (color + vec3(1.0));
		// gamma correct
		color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}
