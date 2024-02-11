/*
 * Example two meshes and two shaders (could also be used for Program 2)
 * includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>
#include <functional>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// my own graphics classes
#include "MeshContainer.h"
#include "Object.h"
#include "function_generator.h"

#define DEF_ANGLETHETA 0.0f // M_PIf / 6.0f
#define DEF_ANGLEPHI -1 *  M_PI_2f + 0.25f
#define SETCOLOR(r, g, b) (glUniform3f(solidColorProg->getUniform("solidColor"), r, g, b))

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

	/* Define Mesh and Object file memories here... */

	// Shape to be used (from  file) - modify to support multiple
	std::map<std::string, shared_ptr<Object>> objects;

	//animation data
	float sTheta = 0;
	float graphT = 0;
	float cloudsize = 1;

	// camera data
	float theta = DEF_ANGLETHETA; // "A" or "D" changes the theta camera angle
	float phi = DEF_ANGLEPHI;  // "W" or "S" changes the phi cmaera angle
	float r = 1;     // "Q" or "E" changes the radius from the origin
	float xtrans = 0;
	float ytrans = 0;
	float ztrans = 0;

	const float speed = 0.02; // sensitivity variable, in degrees per press
	double lastx = 0; // used in mouse controls
	double lasty = 0; // used in mouse controls
	// key detections while pressed. 
	bool keys[GLFW_KEY_LAST] = {false};
	bool mousePressed = false;
	double posX, posY;

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
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");

		// Initialize the GLSL program.
		solidColorProg = make_shared<Program>();
		solidColorProg->setVerbose(true);
		solidColorProg->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/solid_frag.glsl");
		solidColorProg->init();
		solidColorProg->addUniform("P");
		solidColorProg->addUniform("V");
		solidColorProg->addUniform("M");
		solidColorProg->addUniform("solidColor");
		solidColorProg->addAttribute("vertPos");
		solidColorProg->addAttribute("vertNor");
	}

	/**
	 * Define what Mesh objects we have and where their resources are called.  
	*/
	void initGeom(const std::string& resourceDirectory)
	{
		objects["bunny"] = make_shared<Object>(Object(resourceDirectory, "bunny.obj", prog));
		objects["dummy"] = make_shared<Object>(Object(resourceDirectory, "dummy.obj", solidColorProg));
		objects["sphere"] = make_shared<Object>(Object(resourceDirectory, "SmoothSphere.obj", prog));
		objects["cloud"] = make_shared<Object>(Object(resourceDirectory, "cloud.obj", solidColorProg));

		// use DESMOS or any other 3d graphing software to plot this out for yourself!
		funcToObj(
			resourceDirectory, 
			"graph.obj",
			100,
			[](float x, float y) { return 10 * exp(-abs(x)) * exp(-abs(y)) * sin(x) * sin(y); },
			-3.0, 
			3.0, 
			-3.0, 
			3.0
		);
		objects["graph"] = make_shared<Object>(Object(resourceDirectory, "graph.obj", prog));

		funcToObj(
			resourceDirectory,
			"plane.obj",
			5, 
			[](float x, float y) { return 0; },
			-1.0, 
			1.0, 
			-1.0, 
			1.0
		);
		objects["plane"] = make_shared<Object>(Object(resourceDirectory, "plane.obj", solidColorProg));
	}


	/* helper for sending top of the matrix strack to GPU */
	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   }

	/* helper function to set model trasnforms */
  	void setModel(shared_ptr<Program> curS, vec3 trans, float rotY, float rotX, float sc) {
  		mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
  		mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, vec3(1, 0, 0));
  		mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, vec3(0, 1, 0));
  		mat4 ScaleS = glm::scale(glm::mat4(1.0f), vec3(sc));
  		mat4 ctm = Trans*RotX*RotY*ScaleS;
  		glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
  	}

	void render() {
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
		View->pushMatrix();
			View->loadIdentity();

			// My Scene View Defaults. No rotate here please for user control. 
			View->translate(vec3(0,1,-5));
			View->scale(vec3(2,2,2));
			

		Model->loadIdentity();

		solidColorProg->bind();
		//send the projetion and view for solid shader
		glUniformMatrix4fv(solidColorProg->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(solidColorProg->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		//send in the color to use
		glUniform3f(solidColorProg->getUniform("solidColor"), 0.1, 0.2, 0.5);
		solidColorProg->unbind();

		// Draw base Hierarchical person
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));

		// define world manipulations below. 
		Model->translate(vec3(xtrans, ytrans, ztrans));
		Model->rotate(theta, vec3(0,1,0)); // theta rotates around our y-axis
		Model->rotate(phi, vec3(1,0,0)); // phi is around the x-axis
		Model->scale(vec3(r, r, r)); // r is the radius we need to scale our world by.

		// define initial geometry placement here...

		const unsigned int num_dummies = 6;

		// Dummies and their placement
		objects["dummy"]->add_transform(scale(mat4(1.0f), vec3(0.25, 0.25, 0.25)));
		objects["dummy"]->add_transform(rotate(mat4(1.0f), sTheta, vec3(0,0,1)));

		objects["plane"]->add_transform(translate(mat4(1.0f), vec3(0,0,-1)));

		objects["tempdummy"] = objects["dummy"]->copy();
		objects["tempdummy"]->setShader(solidColorProg);
		objects["tempdummy"]->move_to(objects["plane"]);

		objects["tempdummy"]->add_transform(rotate(mat4(1.0f), sin(sTheta), vec3(0,1,0)));
		objects["dummy"]->add_solo_transform(translate(mat4(1.0f), vec3(0,0,sin(sTheta) * 100)));

		for(size_t n = 0; n < num_dummies; ++n)
		{	
			std::string newDummyName = "dummy" + std::to_string(n);
			objects[newDummyName] = objects["tempdummy"]->copy();
			objects[newDummyName]->add_transform(translate(mat4(1.0f), vec3(50, 0, 0)));
			objects[newDummyName]->add_transform(rotate(mat4(1.0f), n * M_PIf * 2 / num_dummies + sTheta, vec3(0,0,1)));

		}
		objects["highdummy"] = objects["dummy"]->copy();
		objects["highdummy"]->setShader(prog);

		objects["cloud"]->add_transform(scale(mat4(1.0f), vec3(0.5, 0.5, 0.5)));
		objects["cloud"]->add_transform(scale(mat4(1.0f), vec3(cloudsize, cloudsize, cloudsize)));
		objects["cloud"]->add_solo_transform(translate(mat4(1.0f), vec3(-0.5, -0.5, 650.0)));
		objects["cloud2"] = objects["cloud"]->copy();
		objects["cloud2"]->add_solo_transform(rotate(mat4(1.0f), M_PI_2f, vec3(0,0,1)));
		objects["cloud2"]->add_solo_transform(translate(mat4(1.0f), vec3(-1000, 0, 0)));
		objects["cloud3"] = objects["cloud"]->copy();
		objects["cloud3"]->add_solo_transform(rotate(mat4(1.0f), - M_PI_2f, vec3(0,0,1)));
		objects["cloud3"]->add_solo_transform(translate(mat4(1.0f), vec3(1000, 0, 0)));

		objects["graph"]->add_transform(rotate(mat4(1.0f), M_PI_4f, vec3(0,0,1)));
		objects["highdummy"]->move_to(objects["graph"]);
		objects["highdummy"]->add_solo_transform(translate(mat4(1.0f), vec3(0,0,400)));

		// Draw Objects in an order here. 
		objects["graph"]->draw(Model);
		objects["highdummy"]->draw(Model);
		prog->unbind();

		solidColorProg->bind();
		SETCOLOR(1.0, 0.2, 0.0);
		for(size_t i = 0; i < num_dummies; ++i)
			objects["dummy" + std::to_string(i)]->draw(Model);
		SETCOLOR(sin(sTheta), cos(sTheta), sin(sTheta) * cos(sTheta));
		objects["dummy"]->draw(Model);
		SETCOLOR(0.0, 0.0, 0.0);
		objects["plane"]->draw(Model);
		SETCOLOR(1.0, 1.0, 1.0);
		objects["cloud"]->draw(Model);
		objects["cloud2"]->draw(Model);
		objects["cloud3"]->draw(Model);
		solidColorProg->unbind();

		//animation update
		sTheta = glfwGetTime();
		cloudsize = 1 + 0.005 * sin((M_E * M_PIf) * glfwGetTime());
		graphT += 0.02;

		// update our values based on keys held. 
		if (keys[GLFW_KEY_A]) {
			theta -= M_PI * speed;
		}
		if (keys[GLFW_KEY_D]) {
			theta += M_PI * speed;
		}
		if (keys[GLFW_KEY_W] ) {
			phi += M_PI * speed;
		}
		if (keys[GLFW_KEY_S]) {
			phi -= M_PI * speed;
		}
		if (keys[GLFW_KEY_Q]) {
			r /= 1 + speed;
		}
		if (keys[GLFW_KEY_E]) {
			r *= 1 + speed;
		}
		if (keys[GLFW_KEY_LEFT]) {
			xtrans -= speed;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			xtrans += speed;
		}
		if (keys[GLFW_KEY_UP]) {
			ytrans += speed;
		}
		if (keys[GLFW_KEY_DOWN]) {
			ytrans -= speed;
		}
		if (keys[GLFW_KEY_R]) {
			theta = DEF_ANGLETHETA;
			phi = DEF_ANGLEPHI;
			r = 1;
			xtrans = 0;
			ytrans = 0;
			ztrans = 0;
		}

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

		for(std::map<std::string, shared_ptr<Object>>::iterator it = objects.begin(); it != objects.end(); ++it) {
			it->second->flush();
		}
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

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
