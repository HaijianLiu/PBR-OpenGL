// Include standard libraries
#include <iostream>
#include <random>
// #include <vector>

// Include header files
#include "opengl.hpp"
// Include class
#include "Camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
// #include "Model.hpp"
#include "Object.hpp"

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	glClearColor(0.0,0.0,0.0,1.0);

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// camera
	Camera camera = Camera();

	// build and compile shaders
	Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/DeferredPBR.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/DeferredPBR.fs.glsl");
	Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.equirectangular_to_cubemap.fs.glsl");
	Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.irradiance_convolution.fs.glsl");
	Shader prefilterShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.prefilter.fs.glsl");
	Shader brdfShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.fs.glsl");
	Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.fs.glsl");

	// model and texture
	Model pbrModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPN_AKM.obj",&pbrShader);
	unsigned int albedoMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Base_Color.tga");
	unsigned int normalMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_DirectX.tga");
	unsigned int metallicMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Metallic.tga");
	unsigned int roughnessMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Roughness.tga");
	unsigned int aoMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Ambient_occlusion.tga");
	unsigned int mraMap = combineTexture(window, metallicMap, roughnessMap, aoMap, 4096);

	Model pbrModel2 = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPN_MK2Grenade.obj",&pbrShader);
	unsigned int albedoMap2 = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPNT_MK2Grenade_Base_Color.tga");
	unsigned int normalMap2 = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPNT_MK2Grenade_Normal_DirectX.tga");
	unsigned int metallicMap2 = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPNT_MK2Grenade_Metallic.tga");
	unsigned int roughnessMap2 = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPNT_MK2Grenade_Roughness.tga");
	unsigned int aoMap2 = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/ChamferZone/WPNT_MK2Grenade_Ambient_occlusion.tga");
	unsigned int mraMap2 = combineTexture(window, metallicMap2, roughnessMap2, aoMap2, 4096);


	// pbr: load the HDR environment map
	unsigned int hdrTexture = loadHDR("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/MonValley_A_LookoutPoint_8k.jpg");
	unsigned int envCubemap = genCubemap(window,hdrTexture,equirectangularToCubemapShader,"equirectangularMap",2048,true);
	unsigned int irradianceMap = genIrradianceMap(window,envCubemap,irradianceShader,"environmentMap",32);
	unsigned int prefilterMap = genPrefilterMap(window,envCubemap,prefilterShader,"environmentMap",128);
	unsigned int brdfLUTTexture = genBRDFLUTTexture(window,brdfShader,512);

	// lights
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 20.0f),
		glm::vec3( 10.0f,  10.0f, 20.0f),
		glm::vec3(-10.0f, -10.0f, 20.0f),
		glm::vec3( 10.0f, -10.0f, 20.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(100.0f, 100.0f, 100.0f),
		glm::vec3(100.0f, 100.0f, 100.0f),
		glm::vec3(100.0f, 100.0f, 100.0f),
		glm::vec3(100.0f, 100.0f, 100.0f)
	};

	// initialize static shader uniforms before rendering
	pbrShader.use();
		// matrix
		glm::mat4 projection = camera.getMatrixProjection();
		pbrShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4();
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		// texture
		pbrShader.setMat4("model", model);
		pbrShader.setInt("albedoMap", 0);
		pbrShader.setInt("normalMap", 1);
		pbrShader.setInt("mraMap", 2);

	backgroundShader.use();
		backgroundShader.setInt("environmentMap", 0);
		backgroundShader.setMat4("projection", projection);


	// init RenderPass object
	RenderPass renderPass = RenderPass(window,5);

	renderPass.shader->use();
		// matrix
		renderPass.shader->setMat4("projection", projection);
		//kernel
		std::vector<glm::vec3> ssaoKernel = genSSAOKernel(4);
		for (unsigned int i = 0; i < ssaoKernel.size(); ++i)
			renderPass.shader->setVec3(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
		// noiseTexture
		unsigned int noiseTexture = genNoiseTexture(4);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		renderPass.shader->setInt("texNoise", 9);
		// IBL
		renderPass.shader->setInt("irradianceMap", 10);
		renderPass.shader->setInt("prefilterMap", 11);
		renderPass.shader->setInt("brdfLUT", 12);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		// lights
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
			renderPass.shader->setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
			renderPass.shader->setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
		}





	// render loop
	while (!glfwWindowShouldClose(window)) {

		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderPass.use();

			// render scene, supplying the convoluted irradiance map to the final shader.
			glm::mat4 view = camera.getMatrixView();
			pbrShader.use();
				pbrShader.setMat4("view", view);
				pbrShader.setVec3("camPos", camera.getPosition());
				model = glm::scale(glm::vec3(0.8f));
				model = glm::translate(glm::vec3(0.0, 0.0, 0.0));
				pbrShader.setMat4("model", model);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, albedoMap);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normalMap);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, mraMap);
			pbrModel.draw();
				model = glm::scale(glm::vec3(0.1f));
				model = glm::translate(glm::vec3(-10.0, -10.0, 0.0));
				pbrShader.setMat4("model", model);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, albedoMap2);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normalMap2);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, mraMap2);
			pbrModel2.draw();

			// draw skybox as last
			backgroundShader.use();
				backgroundShader.setMat4("view", view);
				// skybox cube
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
				// glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
				// glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
				backgroundShader.setInt("environmentMap", 0);
			drawSkybox();

		renderPass.finish();

		renderPass.shader->use();
		renderPass.shader->setVec3("cameraPos", camera.getPosition());

		renderPass.render();


		camera.updateInput(window,deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}
