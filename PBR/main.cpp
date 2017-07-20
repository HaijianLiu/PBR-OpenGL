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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera = Camera();
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	glClearColor(0.9,0.9,0.9,1.0);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.fs.glsl");
	Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.equirectangular_to_cubemap.fs.glsl");
	Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.irradiance_convolution.fs.glsl");
	Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.fs.glsl");

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
	unsigned int hdrTexture = loadHDR("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/Topanga_Forest_B_3k.hdr");
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

	unsigned int envCubemap = loadHDRtoCubemap(window,equirectangularToCubemapShader,hdrTexture,32);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	glm::mat4 projection = camera.getMatrixProjection();
	pbrShader.use();
	pbrShader.setMat4("projection", projection);
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	pbrShader.setFloat("ao", 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			// -----
			processInput(window);

			// render
			// ------
			glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw Cube
			equirectangularToCubemapShader.use();
			equirectangularToCubemapShader.setMat4("view",camera.getMatrixView());
			equirectangularToCubemapShader.setMat4("projection",camera.getMatrixProjection());
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			equirectangularToCubemapShader.setInt("equirectangularMap",0);
			drawCube();

			// Framebuffer test;
			// ------
			// equirectangularToCubemapShader.use();
			// glActiveTexture(GL_TEXTURE0);
			// glBindTexture(GL_TEXTURE_2D, hdrTexture);
			// equirectangularToCubemapShader.setInt("equirectangularMap", 0);
			// equirectangularToCubemapShader.setMat4("projection", captureProjection);
			// int i = ((int)glfwGetTime())%6;
			// equirectangularToCubemapShader.setMat4("view", captureViews[i]);
			// renderCube();

			// render scene, supplying the convoluted irradiance map to the final shader.
			// ------------------------------------------------------------------------------------------

			// render light source (simply re-render sphere at light positions)
			// this looks a bit off as we use the same shader, but it'll make their positions obvious and
			// keeps the codeprint small.
			pbrShader.use();
			pbrShader.setMat4("view", camera.getMatrixView());
			pbrShader.setVec3("camPos", camera.getPosition());

			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
					pbrShader.setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
					pbrShader.setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
					drawSphere();
			}
			// bind pre-computed IBL data
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
			pbrShader.setInt("irradianceMap",0);

			drawSphereGroup(pbrShader,10,10);


	// draw skybox as last
    backgroundShader.use();
    backgroundShader.setMat4("view", camera.getMatrixView());
    backgroundShader.setMat4("projection", camera.getMatrixProjection());
    // skybox cube
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
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
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // float cameraSpeed = 2.5 * deltaTime;
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.ProcessKeyboard(FORWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.ProcessKeyboard(BACKWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.ProcessKeyboard(LEFT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------

// renders (and builds at first invocation) a sphere
// -------------------------------------------------

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------

/*


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	initTime();

	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	// Dark blue background
	glClearColor(0.9,0.9,0.9,1.0);
	// Set input mode GLFW_STICKY_KEYS
	glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);




	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

	// build and compile shaders
	// -------------------------
	Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.pbr.fs.glsl");
	Shader equirectangularToCubemapShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.equirectangular_to_cubemap.fs.glsl");
	Shader irradianceShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.cubemap.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.irradiance_convolution.fs.glsl");
	Shader backgroundShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/2.1.2.background.fs.glsl");

	pbrShader.use();
	pbrShader.setInt("irradianceMap", 0);
	// pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	// pbrShader.setFloat("ao", 1.0f);

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

		// pbr: setup framebuffer
 // ----------------------
 unsigned int captureFBO;
 unsigned int captureRBO;
 glGenFramebuffers(1, &captureFBO);
 glGenRenderbuffers(1, &captureRBO);

 glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
 glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

 // pbr: load the HDR environment map
  // ---------------------------------
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  float *data = stbi_loadf("/Users/haijian/Documents/OpenGL/PBR/PBR/Texture/Topanga_Forest_B_3k.hdr", &width, &height, &nrComponents, 0);
  unsigned int hdrTexture;
  if (data)
  {
      glGenTextures(1, &hdrTexture);
      glBindTexture(GL_TEXTURE_2D, hdrTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
  }
  else
  {
      std::cout << "Failed to load HDR image." << std::endl;
  }

	// pbr: setup cubemap to render to and attach to framebuffer
	    // ---------------------------------------------------------
	    unsigned int envCubemap;
	    glGenTextures(1, &envCubemap);
	    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	    for (unsigned int i = 0; i < 6; ++i)
	    {
	        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	    }
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
			    // ----------------------------------------------------------------------------------------------
			    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			    glm::mat4 captureViews[] =
			    {
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			    };

					// pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }





	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0, 0, 1600, 1200);





    // configure global opengl state
    // -----------------------------
    // glEnable(GL_DEPTH_TEST);

		// lights
    // ------
    // glm::vec3 lightPosition = glm::vec3(0.0f, 20.0f, 10.0f);
    // glm::vec3 lightColor = glm::vec3(150.0f, 150.0f, 150.0f);
    // build and compile shaders
    // -------------------------
    // Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.fs.glsl");
		Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/vertexshader.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/fragmentshader.glsl");
		// Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/dielectricPBR.vertex.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/dielectricPBR.fragment.glsl");
		// Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.1PBR.vertex.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.1PBR.fragment.glsl");

    // load models
    // -----------
    // Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/nanosuit/nanosuit.obj");

		// Model* testModel = new Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/nanosuit/nanosuit.obj", &ourShader);
		// delete testModel;

		// Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPN_AKM.obj",&ourShader);
		// ourModel.loadPBRTextures("WPNT_AKM_Base_Color.tga","WPNT_AKM_DirectX.tga","WPNT_AKM_Metallic.tga","WPNT_AKM_Roughness.tga","WPNT_AKM_Ambient_occlusion.tga");


		// Model grassModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Grass/GrassMWVizwork_cust.obj",&pbrShader);
		// grassModel.loadPBRTextures("Blade_02.jpg","Blade01_B.jpg");

		Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Wood_Log_qdtdP_4K_3d_ms/Aset_wood_log_M_qdtdP_LOD0.obj",&pbrShader);
		ourModel.loadPBRTextures("Aset_wood_log_M_qdtdP_4K_Albedo.jpg","Aset_wood_log_M_qdtdP_4K_Normal_LOD0.jpg","Aset_wood_log_M_qdtdP_4K_Roughness.jpg","Aset_wood_log_M_qdtdP_4K_Cavity.jpg");

		// Model clayModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Soil_Mud_pjEqV0_4K_surface_ms/plane.obj",&ourShader);
		// clayModel.loadPBRTextures("pjEqV_4K_Albedo.jpg","pjEqV_4K_Normal.jpg","pjEqV_4K_Roughness.jpg","pjEqV_4K_AO.jpg");

		// Model sandstoneModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Rock_Sandstone_plras_4K_3d_ms/Aset_rock_sandstone_S_plras_LOD0.obj",&ourShader);
		// sandstoneModel.loadPBRTextures("Aset_rock_sandstone_S_plras_4K_Albedo.jpg","Aset_rock_sandstone_S_plras_4K_Normal_LOD0.jpg","Aset_rock_sandstone_S_plras_4K_Roughness.jpg","Aset_rock_sandstone_S_plras_4K_Cavity.jpg");

		// Model rockCube = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Rock_Sandstone_plww4_4K_3d_ms/Aset_rock_sandstone_S_plww4_LOD0.obj",&ourShader);
		// rockCube.loadPBRTextures("Aset_rock_sandstone_S_plww4_4K_Albedo.jpg","Aset_rock_sandstone_S_plww4_4K_Normal_LOD0.jpg","Aset_rock_sandstone_S_plww4_4K_Roughness.jpg","Aset_rock_sandstone_S_plww4_4K_Cavity.jpg");

		Camera camera = Camera();
		// Object grassObject = Object(&grassModel);
		// Object grassObject2 = Object(&grassModel);
		// Object grassObject3 = Object(&grassModel);
		Object ourObject = Object(&ourModel);
		// Object clayObject = Object(&clayModel);
		// Object sandstoneObject = Object(&sandstoneModel);
		// Object rockCubeObject = Object(&rockCube);


    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// configure global opengl state
    // -----------------------------

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


				// render light source (simply re-render sphere at light positions)
         // this looks a bit off as we use the same shader, but it'll make their positions obvious and
         // keeps the codeprint small.
				//  lightPosition.y = 20 * (float)sin(glfwGetTime());
				//  lightColor = glm::vec3(150.0f, 150.0f, 150.0f) * (float)sin(glfwGetTime());
				//  pbrShader.setVec3("lightPosition", lightPosition);
				//  pbrShader.setVec3("lightColor", lightColor);


        // view/projection transformations
				// camera.updateInput(getKeyboardPress(window));
				camera.updateInput(window,deltaTime);


				// grassObject.scale(3.0);
				// grassObject.draw(camera);
				// grassObject2.scale(3.0);
				// grassObject2.rotate(glm::vec3(0.0,1.0,0.0));
				// grassObject2.draw(camera);
				// grassObject3.scale(3.0);
				// grassObject3.rotate(glm::vec3(0.0,4.0,0.0));
				// grassObject3.draw(camera);

				ourObject.scale(0.2);
				// ourModel.shader = &ourShader;
				ourObject.draw(camera);

				// clayObject.scale(10.0);
				// clayObject.rotate(0.2*currentTime(), glm::vec3(0.0,1.0,0.0));
				// clayObject.draw(camera);

				// sandstoneObject.translate(0.5,-0.5,0.0);
				// sandstoneObject.scale(0.3);
				// sandstoneObject.rotate(0.2*currentTime(), glm::vec3(0.0,1.0,0.0));
				// sandstoneObject.draw(camera);

				// rockCubeObject.scale(0.1);
				// rockCubeObject.translate(0.0,0.0,1.0);
				// rockCubeObject.rotate(0.3*currentTime(), glm::vec3(0.0,1.0,0.0));
				// rockCubeObject.draw(camera);



				// render skybox (render as last to prevent overdraw)
        backgroundShader.use();
        backgroundShader.setMat4("view", camera.getMatrixView());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
        renderCube();




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

	// if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	// 	object->updateInput(turnUp,1 * deltaTime);
	// if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	// 	object->updateInput(turnDown,1 * deltaTime);
	// if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	// 	object->updateInput(turnLeft,1 * deltaTime);
	// if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	// 	object->updateInput(turnRight,1 * deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
// void mouse_callback(GLFWwindow* window, double xpos, double ypos)
// {
//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }
//
//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//     lastX = xpos;
//     lastY = ypos;
//
//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(yoffset);
// }

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

*/
