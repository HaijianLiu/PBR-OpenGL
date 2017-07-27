#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D pass[10];

uniform sampler2D texNoise;
uniform vec3 samples[4];
// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 4;
float radius = 1.0;
float bias = 0.025;
// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1600.0/4.0, 1200.0/4.0);
uniform mat4 projection;

float ambientOcclusion();
vec3 kernelBlur(sampler2D tex);


void main()
{
    vec3 hdrColor = texture(pass[0], TexCoords).rgb;
		// vec3 blur = kernelBlur(pass[0]);

		float occlusion = ambientOcclusion();

		vec3 color = hdrColor * occlusion;
		// color = max(color,vec3(0));
		// // HDR tonemapping
		color = color / (color + vec3(1.0));
		// gamma correct
		color = pow(color, vec3(1.0/2.2));

		// FragColor = vec4(blur, 1.0);
    FragColor = vec4(vec3(occlusion), 1.0);
}


vec3 kernelBlur(sampler2D tex) {
	vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
	vec3 result = vec3(0.0);
	for (int x = -2; x < 2; ++x) {
		for (int y = -2; y < 2; ++y) {
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(tex, TexCoords + offset).rgb;
		}
	}
	return result / (4.0 * 4.0);
}

float ambientOcclusion() {
	// get input for SSAO algorithm
	vec3 fragPos = texture(pass[1], TexCoords).rgb;
	vec3 normal = texture(pass[2], TexCoords).rgb;
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).rgb);
	// create TBN change-of-basis matrix: from tangent-space to view-space
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// iterate over the sample kernel and calculate occlusion factor
	float occlusion = 0.0;

	for(int i = 0; i < kernelSize; ++i) {
		// get sample position
		vec3 sampleTBN = TBN * samples[i]; // from tangent to view-space
		sampleTBN = fragPos + sampleTBN * radius;

		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(sampleTBN, 1.0);
		offset = projection * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		// get sample depth
		float sampleDepth = texture(pass[1], offset.xy).z; // get depth value of kernel sample

		// range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sampleTBN.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	return 1.0 - (occlusion / kernelSize);
}
