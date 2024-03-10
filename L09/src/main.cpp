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

#define SET_MODE(x) (glUniform1f(texProg->getUniform("mode"), x) )
#define SET_SOLIDCOLOR(x, y, z) (glUniform3f(solidColorProg->getUniform("solidColor"), x, y, z))


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

	std::map<std::string, shared_ptr<Texture>> miscTextures;

	// Particle System
	particleSys *thePartSystem;

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

		// Initialize the GLSL program.
		partProg = make_shared<Program>();
		partProg->setVerbose(true);
		partProg->setShaderNames(
			resourceDirectory + "/particle_vert.glsl",
			resourceDirectory + "/particle_frag.glsl");
		if (! partProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		partProg->addUniform("P");
		partProg->addUniform("M");
		partProg->addUniform("V");
		partProg->addAttribute("pColor");
		partProg->addUniform("alphaTexture");
		partProg->addAttribute("vertPos");

		// Initialize the particle system...
		thePartSystem = new particleSys();


		thePartSystem->setInitialVelocityFunc(
			[](glm::vec3 v, Particle p) {
				return 0.1f * (1/glm::distance(v, vec3(0,0,0))) * glm::normalize(glm::vec3(glm::vec4(v, 1.0f) * rotate(mat4(1.0f), M_PI_2f, vec3(0,0,1))));
			}
		);

		thePartSystem->setForce(
			[](glm::vec3 x, Particle p) {
				// the unit perp vector field

				double comp = pow(x.x, 2) + pow(x.y, 2)/4.0;
				comp = (float) comp;
				if(comp == 1) // on the ellipse
				{
					return -4 * M_PIf * x;
				}
				glm::vec3 re = (float)(1 - comp) * x;
				if (comp > 1) // outside of the ellipse
				{
					re = (float)(comp-1) * (glm::vec4(re, 1.0f) * rotate(mat4(1.0f), 0.1f, vec3(0.0f,0.0f,1.0f))); // rotate just a little bit CCW to edge into the ellipse
				}
				else { // inside of the ellipse
					re = (float)(1-comp) * (glm::vec4(re, 1.0f) * rotate(mat4(1.0f), 0.1f, vec3(0.0f,0.0f,1.0f))); // rotate just a little bit CW to edge into ellipse
				}
				re.z = -x.z;
				return re * p.m;
			}
		);
		thePartSystem->setDist(
			[](float t, float a, float b) {
				return glm::vec3(
					(0.9f + (a / 2.5f)) * cos(M_PIf * 2 * t), 
					2*(0.9f + (b / 2.5f)) * sin(M_PIf * 2 * t), 
					a > 0 ? (b * 0.1) : (-b * 0.1)
				);
			}
		);
		thePartSystem->setColorDist(
			[](float t, float a, float b) {
				return glm::vec3(
					0,
					0.5*t + 0.4, 
					0.8*t + 0.4
				);
			}
		);

		thePartSystem->gpuSetup();

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

		// Textures
		miscTextures["Particle"] = make_shared<Texture>(Texture());
		miscTextures["Particle"]->setFilename(resourceDirectory +"/particles" + "/alpha.bmp");
		miscTextures["Particle"]->init();
		miscTextures["Particle"]->setUnit(0);
		miscTextures["Particle"]->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	typedef enum {
		shiny,
		plastic, 
		metalic,
		gold,
		ethereal
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
    			glUniform3f(curS->getUniform("MatAmb"), 0.1 * color.x, 0.1 * color.y, 0.1 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.9, 0.9, 0.9);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);
    		break;
    		case plastic:
    			glUniform3f(curS->getUniform("MatAmb"), 0.1 * color.x, 0.1 * color.y, 0.1 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.1 , 0.1, 0.1);
    			glUniform1f(curS->getUniform("MatShine"), 4.0);
    		break;
    		case metalic:
    			glUniform3f(curS->getUniform("MatAmb"), 0.1 * color.x, 0.1 * color.y, 0.1 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.3, 0.3, 0.3);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
    		break;
			case gold:
				glUniform3f(curS->getUniform("MatAmb"), 0.1 * color.x, 0.1 * color.y, 0.1 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.6 * color.y, 0.6 * color.x, 0.6 * color.z);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
			case ethereal: // high ambient color (near white), no diffusion, high specular
				glUniform3f(curS->getUniform("MatAmb"), 1.1 * color.x, 1.1 * color.y, 0.8 * color.z);
    			glUniform3f(curS->getUniform("MatDif"), 0.0, 0.0, 0.0);
    			glUniform3f(curS->getUniform("MatSpec"), 0.3 * color.x, 0.3 * color.y, 0.3 * color.z);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);
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

			thePartSystem->setCamera(View->topMatrix());

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
			SET_MODE(0);
		texProg->unbind();

		prog->bind();
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
			CHECKED_GL_CALL(glUniform3f(prog->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z));
		prog->unbind();

		partProg->bind();
			miscTextures["Particle"]->bind(partProg->getUniform("alphaTexture"));
			CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix())));
			CHECKED_GL_CALL(glUniformMatrix4fv(partProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix())));
		partProg->unbind();

		// Define World Geometry Here!
		texobjects["BluePortal"]->add_transform(scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f)));
		texobjects["BluePortal"]->add_transform(translate(mat4(1.0f), vec3(0,0,-5)));
		texobjects["BluePortal"]->add_subobj(texobjects["PlanePortalable"]);

		texobjects["BluePortal"]->add_solo_transform(translate(mat4(1.0f), vec3(0,0,0.01)));

		objects["SmoothSphere"]->move_to(lightPosition);

		// x-axis
		VoidBobjects["V1"]->init_transforms();
		VoidBobjects["V1"]->CallMethodOnAll(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		// y-axis
		VoidBobjects["V2"]->init_transforms();
		VoidBobjects["V2"]->CallMethodOnAll(&Object::add_transform, rotate(mat4(1.0f), M_PI_2f, vec3(0,0,1)));
		VoidBobjects["V2"]->CallMethodOnAll(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		// z-axis
		VoidBobjects["V3"]->init_transforms();
		VoidBobjects["V3"]->CallMethodOnAll(&Object::add_transform, rotate(mat4(1.0f), M_PI_2f, vec3(0,1,0)));
		VoidBobjects["V3"]->CallMethodOnAll(&Object::add_transform, translate(mat4(1.0f), vec3(-2,-2,-4)));

		std::cout << glm::to_string(texobjects["BluePortal"]->getWorldCenterPoint()) << std::endl;

		// update our particle system's origin
		thePartSystem->setOrigin(texobjects["BluePortal"]->getWorldCenterPoint());

		// draw objects here... 

		partProg->bind();
			thePartSystem->update();
			thePartSystem->drawMe(partProg);
		partProg->unbind();

		texProg->bind();

			SET_MODE(0); // lighting and shading 

			SetMaterial(texProg, material_t::plastic, vec3(0.1,0.1,0.1));
			//texobjects["PlanePortalable"]->draw(Model);

			SET_MODE(1); // Removing very black pixels from the textures, reserved for portals or other similar textures

			SetMaterial(texProg, material_t::ethereal, vec3(0,0,0.1));
			texobjects["BluePortal"]->draw(Model);

			SET_MODE(2); // textured while unlit
			
		texProg->unbind();

		solidColorProg->bind();
			SET_SOLIDCOLOR(0.5, 0, 0);
			VoidBobjects["V1"]->CallMethodOnAll(&Object::draw, Model);

			SET_SOLIDCOLOR(0, 0.5, 0);
			VoidBobjects["V2"]->CallMethodOnAll(&Object::draw, Model);

			SET_SOLIDCOLOR(0,0,0.5);
			VoidBobjects["V3"]->CallMethodOnAll(&Object::draw, Model);
		solidColorProg->unbind();

		prog->bind();
			SetMaterial(prog, material_t::gold, vec3(1, 1, 0));
			objects["SmoothSphere"]->draw(Model);
		prog->unbind();

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
			thePartSystem->reSet();
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
			it->second->CallMethodOnAll(&Object::flush);
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
