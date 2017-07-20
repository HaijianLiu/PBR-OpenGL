// Include standard libraries
#include <iostream>
// #include <vector>

// Include header files
#include "opengl.hpp"
#include "timecontrol.hpp"
// Include class
#include "Camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
// #include "Model.hpp"
#include "Object.hpp"


void processInput(GLFWwindow *window);

// camera
Camera camera = Camera();
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	glClearColor(0.9,0.9,0.9,1.0);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// build and compile shaders
	// -------------------------
	// Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.fs.glsl");
	// Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.equirectangular_to_cubemap.fs.glsl");
	// Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.irradiance_convolution.fs.glsl");
	// Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.fs.glsl");

	Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.pbr.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.pbr.fs.glsl");
	Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.equirectangular_to_cubemap.fs.glsl");
	Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.irradiance_convolution.fs.glsl");
	Shader prefilterShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.prefilter.fs.glsl");
	Shader brdfShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.fs.glsl");
	Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.fs.glsl");

	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);
	pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	pbrShader.setFloat("ao", 1.0f);

	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	// lights
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3( 10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3( 10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};


	// pbr: load the HDR environment map
	// ---------------------------------
	unsigned int hdrTexture = loadHDR("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/Tropical_Beach_3k.hdr");
	// std::vector<const char*> faces
	// {
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/right.jpg",
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/left.jpg",
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/top.jpg",
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/bottom.jpg",
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/back.jpg",
	// 	"/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/skybox/front.jpg"
	// };
	// unsigned int cubemap = loadCubemap(faces);

	unsigned int envCubemap = genCubemap(window,hdrTexture,equirectangularToCubemapShader,"equirectangularMap",1024,true);
	unsigned int irradianceMap = genIrradianceMap(window,envCubemap,irradianceShader,"environmentMap",32);
	unsigned int prefilterMap = genPrefilterMap(window,envCubemap,prefilterShader,"environmentMap",128);
	unsigned int brdfLUTTexture = genBRDFLUTTexture(window,brdfShader,512);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = camera.getMatrixProjection();
	pbrShader.use();
	pbrShader.setMat4("projection", projection);
	backgroundShader.use();
	backgroundShader.setMat4("projection", camera.getMatrixProjection());

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		// ------
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render scene, supplying the convoluted irradiance map to the final shader.
		// ------------------------------------------------------------------------------------------
		pbrShader.use();
		pbrShader.setMat4("view", camera.getMatrixView());
		pbrShader.setVec3("camPos", camera.getPosition());
		// bind pre-computed IBL data
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];

			pbrShader.setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
			pbrShader.setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

			glm::mat4 model = glm::mat4();
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader.setMat4("model", model);

			drawSphere();
		}

		drawSphereGroup(pbrShader,10,10);


		// draw skybox as last
		backgroundShader.use();
		backgroundShader.setMat4("view", camera.getMatrixView());
		// skybox cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
		// glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
		backgroundShader.setInt("environmentMap", 0);
		drawSkybox();


		camera.updateInput(window,deltaTime);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
