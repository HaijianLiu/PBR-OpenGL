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
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

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
    // glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    // Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.fs.glsl");
		// Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/vertexshader.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/fragmentshader.glsl");
		Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/dielectricPBR.vertex.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/dielectricPBR.fragment.glsl");
		Shader pbrShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.1PBR.vertex.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.1PBR.fragment.glsl");

    // load models
    // -----------
    // Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/nanosuit/nanosuit.obj");

		// Model* testModel = new Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/nanosuit/nanosuit.obj", &ourShader);
		// delete testModel;

		// Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPN_AKM.obj",&ourShader);
		// ourModel.loadPBRTextures("WPNT_AKM_Base_Color.tga","WPNT_AKM_DirectX.tga","WPNT_AKM_Metallic.tga","WPNT_AKM_Roughness.tga","WPNT_AKM_Ambient_occlusion.tga");


		Model grassModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Grass/GrassMWVizwork_cust.obj",&ourShader);
		grassModel.loadPBRTextures("Blade_02.jpg","Blade01_B.jpg");

		Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Wood_Log_qdtdP_4K_3d_ms/Aset_wood_log_M_qdtdP_LOD0.obj",&ourShader);
		ourModel.loadPBRTextures("Aset_wood_log_M_qdtdP_4K_Albedo.jpg","Aset_wood_log_M_qdtdP_4K_Normal_LOD0.jpg","Aset_wood_log_M_qdtdP_4K_Roughness.jpg","Aset_wood_log_M_qdtdP_4K_Cavity.jpg");

		// Model clayModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Soil_Mud_pjEqV0_4K_surface_ms/plane.obj",&ourShader);
		// clayModel.loadPBRTextures("pjEqV_4K_Albedo.jpg","pjEqV_4K_Normal.jpg","pjEqV_4K_Roughness.jpg","pjEqV_4K_AO.jpg");

		// Model sandstoneModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Rock_Sandstone_plras_4K_3d_ms/Aset_rock_sandstone_S_plras_LOD0.obj",&ourShader);
		// sandstoneModel.loadPBRTextures("Aset_rock_sandstone_S_plras_4K_Albedo.jpg","Aset_rock_sandstone_S_plras_4K_Normal_LOD0.jpg","Aset_rock_sandstone_S_plras_4K_Roughness.jpg","Aset_rock_sandstone_S_plras_4K_Cavity.jpg");

		// Model rockCube = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/Rock_Sandstone_plww4_4K_3d_ms/Aset_rock_sandstone_S_plww4_LOD0.obj",&ourShader);
		// rockCube.loadPBRTextures("Aset_rock_sandstone_S_plww4_4K_Albedo.jpg","Aset_rock_sandstone_S_plww4_4K_Normal_LOD0.jpg","Aset_rock_sandstone_S_plww4_4K_Roughness.jpg","Aset_rock_sandstone_S_plww4_4K_Cavity.jpg");

		Camera camera = Camera();
		Object grassObject = Object(&grassModel);
		Object grassObject2 = Object(&grassModel);
		Object grassObject3 = Object(&grassModel);
		Object ourObject = Object(&ourModel);
		// Object clayObject = Object(&clayModel);
		// Object sandstoneObject = Object(&sandstoneModel);
		// Object rockCubeObject = Object(&rockCube);


    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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




        // view/projection transformations
				// camera.updateInput(getKeyboardPress(window));
				camera.updateInput(window,deltaTime);


				grassObject.scale(3.0);
				grassObject.draw(camera);
				grassObject2.scale(3.0);
				grassObject2.rotate(glm::vec3(0.0,1.0,0.0));
				grassObject2.draw(camera);
				grassObject3.scale(3.0);
				grassObject3.rotate(glm::vec3(0.0,4.0,0.0));
				grassObject3.draw(camera);

				ourObject.scale(0.2);
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
