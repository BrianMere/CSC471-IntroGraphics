/*
 * Example two meshes and two shaders (could also be used for Program 2)
 * includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */
#ifndef _USE_GNU
	#define _USE_GNU
#endif 

#include <iostream>
#include <glad/glad.h>
#include <functional>
#include <chrono>

#include "Materials.h"
#include "GLSL.h"
#include "Program.h"
#include "Texture.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "Spline.h"
#include "particleSys.h"

#define GLM_SWIZZLE

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// my own graphics classes
#include "MeshContainer.h"
#include "Object.h"
#include "Camera.h"
#include "TextureObject.h"
#include "function_generator.h"
#include "VectorObj.h"
#include "PortalObject.h"


using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

	WindowManager * windowManager = nullptr;

	/* Define shaders here ... */

	// Our shader program
	std::shared_ptr<Program> prog;
	// Our shader program, no lighting
	std::shared_ptr<Program> solidColorProg;
	// Our texture shader program
	std::shared_ptr<Program> texProg;
	// Particle shader
	std::shared_ptr<Program> partProg;

	/* Define Mesh and Object file memories here... */

	// Shape to be used (from  file) - modify to support multiple
	std::map<std::string, shared_ptr<Object>> objects;
	std::map<std::string, shared_ptr<TextureObject>> texobjects;
	std::map<std::string, shared_ptr<Bobject<>>> VoidBobjects;
	std::map<std::string, shared_ptr<PortalObj> > portalObjs;

	std::map<std::string, shared_ptr<Texture>> miscTextures;

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
		CHECKED_GL_CALL(glClearColor(.12f, .34f, .56f, 1.0f));

		// Enable z-buffer test.
		CHECKED_GL_CALL(glEnable(GL_DEPTH_TEST));
		CHECKED_GL_CALL(glEnable(GL_BLEND));
		CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		CHECKED_GL_CALL(glPointSize(24.0f));

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

		

		// init splines up and down
       	splinepath[0] = Spline(glm::vec3(-1,0.5,1), glm::vec3(-0.5,1,1), glm::vec3(0.5, 0 , 1), glm::vec3(1,0.5,1), 5);
       	splinepath[1] = Spline(glm::vec3(1,0.5,1), glm::vec3(0.66, 0.66,1), glm::vec3(0.33, 0.33, 1), glm::vec3(0,0,1), 5);
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

		texobjects["PlaneUnportalable"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"plane.obj",
			texProg, 
			"/black_floor.jpg"
		));

		texobjects["SmoothSphere"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"sphereWTex.obj",
			texProg, 
			"/ar_lunacy.jpg"
		));
		// add another test object
		objects["SmoothSphere"] = make_shared<Object>(Object(
			resourceDirectory + "/misc",
			"sphereWTex.obj",
			prog
		));

		texobjects["SkyBox"] = make_shared<TextureObject>(TextureObject(
			resourceDirectory + "/misc",
			"cube.obj",
			texProg, 
			"/ar_lunacy.jpg"
		));

		VoidBobjects["V1"] = make_shared<VectorObj>(VectorObj(
			resourceDirectory, 
			solidColorProg
		));
		VoidBobjects["V1"]->init();

		VoidBobjects["V2"] = make_shared<VectorObj>(VectorObj(
			resourceDirectory, 
			solidColorProg
		));
		VoidBobjects["V2"]->init();

		VoidBobjects["V3"] = make_shared<VectorObj>(VectorObj(
			resourceDirectory, 
			solidColorProg
		));
		VoidBobjects["V3"]->init();

		// FBO's
		portalObjs["BluePortal"] = make_shared<PortalObj>(PortalObj(
			resourceDirectory, 
			texProg,
			vec3(0.0,0.0,1.0)
		));

		portalObjs["OrangePortal"] = make_shared<PortalObj>(PortalObj(
			resourceDirectory, 
			texProg, 
			vec3(1.0,0.5,0.0)
		));

		for(std::map<std::string, shared_ptr<PortalObj>>::iterator it = portalObjs.begin(); it != portalObjs.end(); ++it) {
			it->second->init();
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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		glm::vec3 lightPosition = vec3(2.0, 2.0, -4.0); // center a light really far away super high...

		// Initialize Shader Values...

		solidColorProg->bind();
			CHECKED_GL_CALL(glUniformMatrix4fv(solidColorProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(solidColorProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(solidColorProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
			CHECKED_GL_CALL(glUniform3f(solidColorProg->getUniform("solidColor"), 0.1, 0.2, 0.5));
		solidColorProg->unbind();

		texProg->bind();
			CHECKED_GL_CALL(glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
			CHECKED_GL_CALL(glUniform3f(texProg->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z));
			SET_MODE(texProg, 0);
		texProg->unbind();

		prog->bind();
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
			CHECKED_GL_CALL(glUniform3f(prog->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z));
		prog->unbind();

		for(std::map<std::string, shared_ptr<PortalObj>>::iterator it = portalObjs.begin(); it != portalObjs.end(); ++it) {
			it->second->partProg->bind();
				CHECKED_GL_CALL(glUniformMatrix4fv(it->second->partProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
				CHECKED_GL_CALL(glUniformMatrix4fv(it->second->partProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
				CHECKED_GL_CALL(glUniformMatrix4fv(it->second->partProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
			it->second->partProg->unbind();

			// No need to bind anything for the passProg shader... 
		}

		// Define World Geometry Here!

		objects["SmoothSphere"]->move_to(lightPosition);

		// x-axis
		VoidBobjects["V1"]->init_transforms();
		VoidBobjects["V1"]->CallMethodOnAllObjs(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		// y-axis
		VoidBobjects["V2"]->init_transforms();
		VoidBobjects["V2"]->CallMethodOnAllObjs(&Object::add_transform, rotate(mat4(1.0f), M_PI_2f, vec3(0,0,1)));
		VoidBobjects["V2"]->CallMethodOnAllObjs(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		// z-axis
		VoidBobjects["V3"]->init_transforms();
		VoidBobjects["V3"]->CallMethodOnAllObjs(&Object::add_transform, rotate(mat4(1.0f), M_PI_2f, vec3(0,1,0)));
		VoidBobjects["V3"]->CallMethodOnAllObjs(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		// Portals
		portalObjs["BluePortal"]->init_transforms();
		portalObjs["BluePortal"]->CallMethodOnAllTexobjs<TextureObject>(&TextureObject::move_to, vec3(-2, 0,-5));
		portalObjs["BluePortal"]->thePartSystem->setOrigin(vec3(-2, 0, -5));

		portalObjs["OrangePortal"]->init_transforms();
		portalObjs["OrangePortal"]->CallMethodOnAllTexobjs<TextureObject>(&TextureObject::move_to, vec3(2, 0,-5));
		portalObjs["OrangePortal"]->thePartSystem->setOrigin(vec3(2, 0, -5));

		// draw objects here... 

		// Portal FBO Rendering
		portalObjs["BluePortal"]->fbobjs["PortalHole"]->setupFBO(View, Model, {solidColorProg, prog});
			drawTheWorld(Model);
			portalObjs["OrangePortal"]->drawMe(Model);
		portalObjs["BluePortal"]->fbobjs["PortalHole"]->renderFBO(Model, {solidColorProg, prog});

		portalObjs["OrangePortal"]->fbobjs["PortalHole"]->setupFBO(View, Model, {solidColorProg, prog});
			drawTheWorld(Model);
			portalObjs["BluePortal"]->drawMe(Model);
		portalObjs["OrangePortal"]->fbobjs["PortalHole"]->renderFBO(Model, {solidColorProg, prog});

		// now show the screen (we don't output the image of texBuf[2] here, we're just saving it. )
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawTheWorld(Model);
		portalObjs["BluePortal"]->drawMe(Model);
		portalObjs["OrangePortal"]->drawMe(Model);

		// Camera Updates
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
		if (keys[GLFW_KEY_P]) {
			for(auto it : portalObjs)
			{
				it.second->fbobjs["PortalHole"]->writeImageFile(it.first);
			}
			//thePartSystem->reSet();
		}

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

		// Flush current transformations.
		for(std::map<std::string, shared_ptr<Object>>::iterator it = objects.begin(); it != objects.end(); ++it) {
			it->second->flush();
		}
		for(std::map<std::string, shared_ptr<TextureObject>>::iterator it = texobjects.begin(); it != texobjects.end(); ++it) {
			it->second->flush();
		}
		for(std::map<std::string, shared_ptr<Bobject<>>>::iterator it = VoidBobjects.begin(); it != VoidBobjects.end(); ++it) {
			it->second->CallMethodOnAllObjs<Object>(&Object::flush);
		}
		for(std::map<std::string, shared_ptr<PortalObj>>::iterator it = portalObjs.begin(); it != portalObjs.end(); ++it) {
			it->second->CallMethodOnAllTexobjs<Object>(&Object::flush);
			it->second->thePartSystem->setOrigin(vec3(0,0,0));
		}

		// Debugging: Print out our current position
		std::cout << sceneCamera.toString() << std::endl;
	}

	void drawTheWorld(std::shared_ptr<MatrixStack> Model)
	{
		solidColorProg->bind();
			SET_SOLIDCOLOR(solidColorProg, 0.5, 0, 0);
			//VoidBobjects["V1"]->CallMethodOnAllObjs(&Object::draw, Model);

			SET_SOLIDCOLOR(solidColorProg, 0, 0.5, 0);
			//VoidBobjects["V2"]->CallMethodOnAllObjs(&Object::draw, Model);

			SET_SOLIDCOLOR(solidColorProg, 0,0,0.5);
			//VoidBobjects["V3"]->CallMethodOnAllObjs(&Object::draw, Model);
		solidColorProg->unbind();

		prog->bind();
			SetMaterial(prog, material_t::gold, vec3(1, 1, 0));
			//objects["SmoothSphere"]->draw(Model);
		prog->unbind();
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
