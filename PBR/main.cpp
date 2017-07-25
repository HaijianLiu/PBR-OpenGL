// Include standard libraries
#include <iostream>
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

bool bloom = true;
float exposure = 1.0f;

int main() {

	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	glClearColor(0.0,0.0,0.0,1.0);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// camera
	Camera camera = Camera();

	// build and compile shaders
	Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.pbr.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.2.pbr.fs.glsl");
	Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.equirectangular_to_cubemap.fs.glsl");
	Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.irradiance_convolution.fs.glsl");
	Shader prefilterShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.prefilter.fs.glsl");
	Shader brdfShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.brdf.fs.glsl");
	Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.2.1.background.fs.glsl");

	// Shader shaderBlur = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/7.blur.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/7.blur.fs.glsl");

	Model pbrModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPN_AKM.obj",&pbrShader);
	unsigned int albedoMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Base_Color.tga");
	unsigned int normalMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_DirectX.tga");
	unsigned int metallicMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Metallic.tga");
	unsigned int roughnessMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Roughness.tga");
	unsigned int aoMap = loadTexture("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPNT_AKM_Ambient_occlusion.tga");

	// pbr: load the HDR environment map
	unsigned int hdrTexture = loadHDR("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/NarrowPath_8k.jpg");
	unsigned int envCubemap = genCubemap(window,hdrTexture,equirectangularToCubemapShader,"equirectangularMap",2048,true);
	unsigned int irradianceMap = genIrradianceMap(window,envCubemap,irradianceShader,"environmentMap",32);
	unsigned int prefilterMap = genPrefilterMap(window,envCubemap,prefilterShader,"environmentMap",128);
	unsigned int brdfLUTTexture = genBRDFLUTTexture(window,brdfShader,512);

	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);
	pbrShader.setInt("albedoMap", 3);
	pbrShader.setInt("normalMap", 4);
	pbrShader.setInt("metallicMap", 5);
	pbrShader.setInt("roughnessMap", 6);
	pbrShader.setInt("aoMap", 7);

	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	RenderPass renderPass = RenderPass(window,2);


	// lights
	// ------
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
	// --------------------------------------------------
	glm::mat4 projection = camera.getMatrixProjection();
	pbrShader.use();
	pbrShader.setMat4("projection", projection);
	backgroundShader.use();
	backgroundShader.setMat4("projection", projection);

	// render loop
	// -----------
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
		pbrShader.use();
		glm::mat4 view = camera.getMatrixView();
		pbrShader.setMat4("view", view);
		pbrShader.setVec3("camPos", camera.getPosition());
		glm::mat4 model = glm::mat4();
		model = glm::scale(model, glm::vec3(0.8f));
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    pbrShader.setMat4("model", model);

		// bind pre-computed IBL data
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, albedoMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, metallicMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, roughnessMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, aoMap);

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			pbrShader.setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
			pbrShader.setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
		}

		pbrModel.draw();

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		// 2. blur bright fragments with two-pass Gaussian Blur
		// --------------------------------------------------
		// bool horizontal = true, first_iteration = true;
		// unsigned int amount = 10;
		// shaderBlur.use();
		// for (unsigned int i = 0; i < amount; i++)
		// {
		// 		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		// 		shaderBlur.setInt("horizontal", horizontal);
		// 		glBindTexture(GL_TEXTURE_2D, first_iteration ? renderPass.pass[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		// 		drawQuad();
		// 		horizontal = !horizontal;
		// 		if (first_iteration)
		// 				first_iteration = false;
		// }
		// glBindFramebuffer(GL_FRAMEBUFFER, 0);




		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		// --------------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderPass.shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderPass.pass[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderPass.pass[1]);
		// renderPass.shader->setInt("bloom", bloom);
		// renderPass.shader->setFloat("exposure", exposure);
		drawQuad();

			// std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;


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
