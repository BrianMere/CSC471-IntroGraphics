/*
 * Example two meshes and two shaders (could also be used for Program 2)
 * includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */
#ifndef _USE_GNU
	#define _USE_GNU
#endif 

#define GLM_ENABLE_EXPERIMENTAL // for string functions..

#include <iostream>
#include <glad/glad.h>
#include <functional>
#include <chrono>

#include "GLSL.h"
#include "Program.h"
#include "Texture.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Spline.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> // for string casts

// my own graphics classes
#include "MeshContainer.h"
#include "Object.h"
#include "Camera.h"
#include "VectorObj.h"
#include "TextureObject.h"
#include "function_generator.h"

#define SET_MODE(x) (glUniform1f(texProg->getUniform("mode"), x) )


using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

	WindowManager * windowManager = nullptr;

	/* Define shaders here ... */

	// Our shader program
	std::shared_ptr<Program> prog;
	// Our shader program
	std::shared_ptr<Program> solidColorProg;
	// Our texture shader program
	std::shared_ptr<Program> texProg;

	/* Define Mesh and Object file memories here... */

	// Shape to be used (from  file) - modify to support multiple
	std::map<std::string, shared_ptr<Object>> objects;
	std::map<std::string, shared_ptr<TextureObject>> texobjects;
	std::map<std::string, shared_ptr<Texture>> textures;
	std::map<std::string, shared_ptr<Object>> copied_objs; // store any copy() objects here to save on memory. 

	//animation data
	float sTheta = 0;
	float graphT = 0;

	// camera data
	Camera sceneCamera = Camera();
	Spline splinepath[2];
	bool isTour = false;

	double lastx = 0; // used in mouse controls
	double lasty = 0; // used in mouse controls
	// key detections while pressed. 
	bool keys[GLFW_KEY_LAST] = {false};
	bool mousePressed = false;
	double posX, posY; // used in camera click controls

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}

		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			mousePressed = true;
			glfwGetCursorPos(window, &posX, &posY);
			lastx = posX;
			lasty = posY;
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
		if (action == GLFW_RELEASE) 
		{
			mousePressed = false;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY) 
	{
		sceneCamera.rotateCamera(deltaX, deltaY);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/lighting_vert.glsl", resourceDirectory + "/lighting_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("MatAmb");
		prog->addUniform("lightPos");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		// added
		prog->addUniform("MatDif");
		prog->addUniform("MatSpec");
		prog->addUniform("MatShine");

		// Initialize the GLSL program.
		solidColorProg = make_shared<Program>();
		solidColorProg->setVerbose(true);
		solidColorProg->setShaderNames(resourceDirectory + "/solid_vert.glsl", resourceDirectory + "/solid_frag.glsl");
		solidColorProg->init();
		solidColorProg->addUniform("P");
		solidColorProg->addUniform("V");
		solidColorProg->addUniform("M");
		solidColorProg->addUniform("solidColor");
		solidColorProg->addAttribute("vertPos");
		solidColorProg->addAttribute("vertNor");

		// Initialize the GLSL program that we will use for texture mapping
		texProg = make_shared<Program>();
		texProg->setVerbose(true);
		texProg->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag.glsl");
		texProg->init();
		texProg->addUniform("P");
		texProg->addUniform("V");
		texProg->addUniform("M");
		texProg->addUniform("Texture0");
		texProg->addAttribute("vertPos");
		texProg->addAttribute("vertNor");
		texProg->addAttribute("vertTex");
		// lighting for textures
		texProg->addUniform("MatAmb");
		texProg->addUniform("lightPos");
		texProg->addUniform("MatDif");
		texProg->addUniform("MatSpec");
		texProg->addUniform("MatShine");
		texProg->addUniform("mode");
	}

	/**
	 * Define what Mesh objects we have and where their resources are called.  
	*/
	void initGeom(const std::string& resourceDirectory)
	{
		texobjects["PortalDoor"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal", 
			"portal_door_combined_model.obj", 
			texProg, 
			"/portal_door_Diff.jpg"
		));

		texobjects["PortalSwitch"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"switch.obj",
			texProg, 
			"/switch.jpg"
		));

		texobjects["PortalButtonOn"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"portal_button.obj",
			texProg, 
			"/portal_button_clean_on.png"
		));

		texobjects["PortalButtonOff"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"portal_button.obj",
			texProg, 
			"/portal_button_clean_off.png"
		));

		texobjects["CompanionCube"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"EDITOR_companion_cube.obj",
			texProg, 
			"/EDITOR_companion_cube.png"
		));

		// generate a ground obj 
		funcToObj(
			resourceDirectory + "/misc",
			"plane.obj",
			1,
			[](float x, float y) { return 0; },
			-1.0, 
			1.0, 
			-1.0, 
			1.0
		);

		funcToObj(
			resourceDirectory + "/portal",
			"portal_plane.obj", 
			1, 
			[](float x, float y) { return 0; },
			-1.0, 
			1.0, 
			-2.0, 
			2.0
		);

		texobjects["PlanePortalable"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"plane.obj",
			texProg, 
			"/wall_texture.png"
		));

		texobjects["PlaneUnportalable"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"plane.obj",
			texProg, 
			"/black_floor.jpg"
		));

		texobjects["BluePortal"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"portal_plane.obj",
			texProg, 
			"/blue_swirl.jpg"
		));

		texobjects["OrangePortal"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/portal",
			"portal_plane.obj",
			texProg, 
			"/orange_swirl.jpg"
		));

		texobjects["SmoothSphere"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"sphereWTex.obj",
			texProg, 
			"/ar_lunacy.jpg"
		));

		texobjects["SkyBox"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"cube.obj",
			texProg, 
			"/ar_lunacy.jpg"
		));

	 	// init splines up and down
       	splinepath[0] = Spline(glm::vec3(-1,0.5,1), glm::vec3(-0.5,1,1), glm::vec3(0.5, 0 , 1), glm::vec3(1,0.5,1), 5);
       	splinepath[1] = Spline(glm::vec3(1,0.5,1), glm::vec3(0.66, 0.66,1), glm::vec3(0.33, 0.33, 1), glm::vec3(0,0,1), 5);
	}

	typedef enum {
		shiny,
		plastic, 
		metalic,
		gold
	} material_t;

	/**
	 * helper function to pass material data to the GPU.
	 * 
	 * Review material_t for supported materials to pass in. 
	 * Color is an associated RGB value. 
	 * */
	void SetMaterial(shared_ptr<Program> curS, material_t material, vec3 color) {

    	switch (material) {
    		case shiny:
    			glUniform3f(curS->getUniform("MatAmb"), 0.5 * color.x, 0.5 * color.y, 0.5 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.9, 0.9, 0.9);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);
    		break;
    		case plastic:
    			glUniform3f(curS->getUniform("MatAmb"), 0.3 * color.x, 0.3 * color.y, 0.3 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.1 , 0.1, 0.1);
    			glUniform1f(curS->getUniform("MatShine"), 4.0);
    		break;
    		case metalic:
    			glUniform3f(curS->getUniform("MatAmb"), 0.2 * color.x, 0.2 * color.y, 0.2 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.3, 0.3, 0.3);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
    		break;
			case gold:
				glUniform3f(curS->getUniform("MatAmb"), 0.1 * color.x, 0.1 * color.y, 0.1 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.6 * color.y, 0.6 * color.x, 0.6 * color.z);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
    		break;
  		}
	}

	/**
	 * Update camera based on if we are doing a camera tour of our scene currently
	*/
	void updateUsingCameraPath(float frametime)  {

   	  if (isTour) {
       if(!splinepath[0].isDone()){
       		splinepath[0].update(frametime);
            sceneCamera.setPosition(splinepath[0].getPosition());
        } else if (!splinepath[1].isDone()) {
            splinepath[1].update(frametime);
            sceneCamera.setPosition(splinepath[1].getPosition());
        }
		else {
			isTour = false;
		}
      }
   	}

	void render(float deltaTime) {
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the matrix stack for Lab 6
		float aspect = width/(float)height;

		// Create the matrix stacks 
		auto Projection = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

		// View is global translation along negative z for now
		updateUsingCameraPath(deltaTime);
		View->pushMatrix();
			View->loadIdentity();
			View->multMatrix(sceneCamera.getLookAtMatrix());

		Model->loadIdentity();

		glm::vec3 lightPosition = vec3(0.0, 1.0, 0.0); // center a light really far away super high...

		solidColorProg->bind();
		//send the projetion and view for solid shader
		glUniformMatrix4fv(solidColorProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(solidColorProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(solidColorProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		//send in the color to use
		glUniform3f(solidColorProg->getUniform("solidColor"), 0.1, 0.2, 0.5);
		solidColorProg->unbind();

		texProg->bind();
		glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glUniform3f(texProg->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
		SET_MODE(0);
		texProg->unbind();

		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glUniform3f(prog->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
		prog->unbind();

		texobjects["SmoothSphere"]->add_transform(scale(mat4(1.0f), vec3(2.0, 2.0, 2.0)));
		texobjects["SmoothSphere"]->add_transform(translate(mat4(1.0f), vec3(1.0, 1.0, 1.0)));

		if(texobjects.count("CompanionCube2") == 0)
			texobjects["CompanionCube2"] = texobjects["CompanionCube"]->copy();

		cout << glm::to_string(texobjects["CompanionCube2"]->getWorldCenterPoint()) << std::endl;

		texobjects["SmoothSphere"]->add_subobj(texobjects["CompanionCube2"]);
		cout << glm::to_string(texobjects["CompanionCube2"]->getWorldCenterPoint()) << std::endl;

		texobjects["CompanionCube2"]->add_transform(translate(mat4(1.0f), vec3(1, 1, -2)));
		cout << glm::to_string(texobjects["CompanionCube2"]->getWorldCenterPoint()) << std::endl;

		texobjects["CompanionCube2"]->move_to(texobjects["SmoothSphere"]);
		cout << glm::to_string(texobjects["CompanionCube2"]->getWorldCenterPoint()) << std::endl;


		texobjects["CompanionCube"]->add_transform(rotate(mat4(1.0f), M_PI_2f, vec3(1,1,1)));

		

		// draw objects here... 
		texProg->bind();

			SET_MODE(0); // lighting and shading 
			
			

			SET_MODE(1); // remove lighting while removing very black pixels, reserved for portals or other similar textures

			SET_MODE(2); // textured while unlit

			SetMaterial(texProg, plastic, vec3(0.1, 0.1, 0.1));
			texobjects["SmoothSphere"]->draw(Model);
			//texobjects["PlanePortalable"]->draw(Model);
			texobjects["CompanionCube"]->draw(Model);
		texProg->unbind();

		solidColorProg->bind();
			//VectorObj(resourceDirector).parentObj->draw(Model);
		solidColorProg->unbind();

		//animation update
		sTheta = glfwGetTime();
		graphT += 0.02;


		sceneCamera.rotSpeed = 0.02f;
		sceneCamera.transSpeed = 0.04f;
		if (keys[GLFW_KEY_W]) {
			sceneCamera.translateCamera(1, 0);
		}
		if (keys[GLFW_KEY_A]) {
			sceneCamera.translateCamera(0, -1);
		}
		if (keys[GLFW_KEY_S]) {
			sceneCamera.translateCamera(-1, 0);
		}
		if (keys[GLFW_KEY_D]) {
			sceneCamera.translateCamera(0, 1);
		}
		if (keys[GLFW_KEY_R]) {
			sceneCamera.resetCamera();
		}

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

		for(std::map<std::string, shared_ptr<Object>>::iterator it = objects.begin(); it != objects.end(); ++it) {
			it->second->flush();
		}
		for(std::map<std::string, shared_ptr<TextureObject>>::iterator it = texobjects.begin(); it != texobjects.end(); ++it) {
			it->second->flush();
		}

		// Debugging: Print out our current position
		std::cout << sceneCamera.toString() << std::endl;
	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	auto nextLastTime = chrono::high_resolution_clock::now();
	auto lastTime = nextLastTime;

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{

		// save current time for next frame
		auto nextLastTime = chrono::high_resolution_clock::now();

		// get time since last frame
		float deltaTime =
			chrono::duration_cast<std::chrono::microseconds>(
				chrono::high_resolution_clock::now() - lastTime)
				.count();
		// convert microseconds (weird) to seconds (less weird)
		deltaTime *= 0.000001;

		// reset lastTime so that we can calculate the deltaTime
		// on the next frame
		lastTime = nextLastTime;

		// Render scene.
		application->render(deltaTime);

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
