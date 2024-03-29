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
#include "Texture.h"
#include "TextureObject.h"
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
	// Our texture shader program
	std::shared_ptr<Program> texProg;

	std::shared_ptr<Texture> texture0, texture1;

	/* Define Mesh and Object file memories here... */

	// Shape to be used (from  file) - modify to support multiple
	std::map<std::string, shared_ptr<Object>> objects;
	std::map<std::string, shared_ptr<TextureObject>> texobjects;


	//animation data
	float sTheta = 0;
	float graphT = 0;
	float cloudsize = 1;

	// camera data
	float theta = DEF_ANGLETHETA; 
	float phi = DEF_ANGLEPHI;  
	float xtrans = 0;
	float ztrans = 0;
	float lightPos = 0; // number of units to transform lights to the right...

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
			lightPos -= speed;
		}
		if (keys[GLFW_KEY_E]) {
			lightPos += speed;
		}
		if (keys[GLFW_KEY_LEFT]) {
			xtrans -= speed;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			xtrans += speed;
		}
		if (keys[GLFW_KEY_UP]) {
			ztrans += speed;
		}
		if (keys[GLFW_KEY_DOWN]) {
			ztrans -= speed;
		}
		if (keys[GLFW_KEY_R]) {
			theta = DEF_ANGLETHETA;
			phi = DEF_ANGLEPHI;
			xtrans = 0;
			ztrans = 0;
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


		//read in a load the texture
		texture0 = make_shared<Texture>();
  		texture0->setFilename(resourceDirectory + "/lol.jpg");
  		texture0->init();
  		texture0->setUnit(0);
  		texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		texture1 = make_shared<Texture>();
  		texture1->setFilename(resourceDirectory + "/grass.jpg");
  		texture1->init();
  		texture1->setUnit(0);
  		texture1->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}

	/**
	 * Define what Mesh objects we have and where their resources are called.  
	*/
	void initGeom(const std::string& resourceDirectory)
	{
		objects["bunny"] = make_shared<Object>(Object(resourceDirectory, "bunny.obj", prog));
		objects["dummy"] = make_shared<Object>(Object(resourceDirectory, "dummy.obj", prog));
		objects["sphere"] = make_shared<Object>(Object(resourceDirectory, "SmoothSphere.obj", prog));
		objects["cloud"] = make_shared<Object>(Object(resourceDirectory, "cloud.obj", prog));

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
			2,
			[](float x, float y) { return 0; },
			-1.0, 
			1.0, 
			-1.0, 
			1.0
		);
		texobjects["plane"] = make_shared<TextureObject>(TextureObject(resourceDirectory, "plane.obj", texProg, texture1));

		objects["sphereNoNorm"] = make_shared<Object>(Object(resourceDirectory, "icoNoNormals.obj", prog));

		//objects["sphereTex"] = make_shared<Object>(Object(resourceDirectory, "sphereWTex.obj", prog));
		texobjects["sphereTex"] = make_shared<TextureObject>(TextureObject(resourceDirectory, "sphereWTex.obj", texProg, texture0));
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
    			glUniform3f(curS->getUniform("MatAmb"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatDif"), 0.96, 0.96, 0.96);
    			glUniform3f(curS->getUniform("MatSpec"), 1.0, 1.0, 1.0);
    			glUniform1f(curS->getUniform("MatShine"), 120.0);
    		break;
    		case plastic:
    			glUniform3f(curS->getUniform("MatAmb"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatDif"), 0.38 * color.x, 0.38 * color.y, 0.38 * color.z);
    			glUniform3f(curS->getUniform("MatSpec"), 0.1 * color.z, 0.1 * color.x, 0.1 * color.y);
    			glUniform1f(curS->getUniform("MatShine"), 4.0);
    		break;
    		case metalic:
    			glUniform3f(curS->getUniform("MatAmb"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatDif"), 0.46 * color.x, 0.46 * color.y, 0.46 * color.z);
    			glUniform3f(curS->getUniform("MatSpec"),0.6 * color.x, 0.6 * color.y, 0.6 * color.z);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
    		break;
			case gold:
				glUniform3f(curS->getUniform("MatAmb"), color.x, color.y, color.z);
    			glUniform3f(curS->getUniform("MatDif"), 0.46 * color.x, 0.46 * color.y, 0.46 * color.z);
    			glUniform3f(curS->getUniform("MatSpec"),0.6 * color.y, 0.6 * color.x, 0.6 * color.z);
    			glUniform1f(curS->getUniform("MatShine"), 27.0);
    		break;
  		}
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

			// My Scene View Defaults, along with camera control.
			View->translate(vec3(0 + xtrans,1,-5 + ztrans));
			View->scale(vec3(2,2,2));
			View->rotate(theta, vec3(0,1,0)); // theta rotates around our y-axis
			View->rotate(phi, vec3(1,0,0)); // phi is around the x-axis
			

		Model->loadIdentity();

		glm::vec3 lightPosition = vec3(-2.0 + lightPos, 2.0, 1.0);

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

		texProg->unbind();

		// Draw base Hierarchical person
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glUniform3f(prog->getUniform("lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);

		// define initial geometry placement here...

		const unsigned int num_dummies = 6;

		// Dummies and their placement
		objects["dummy"]->add_transform(scale(mat4(1.0f), vec3(0.25, 0.25, 0.25)));
		objects["dummy"]->add_transform(rotate(mat4(1.0f), sTheta, vec3(0,0,1)));

		texobjects["plane"]->add_transform(translate(mat4(1.0f), vec3(0,0,-1)));

		objects["tempdummy"] = objects["dummy"]->copy();
		objects["tempdummy"]->setShader(solidColorProg);
		objects["tempdummy"]->move_to(texobjects["plane"]);

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

		texobjects["sphereTex"]->add_transform(scale(mat4(1.0f), vec3(0.25, 0.25, 0.25)));
		texobjects["sphereTex"]->add_transform(rotate(mat4(1.0f), M_PI_2f, vec3(1,0,0)));
		texobjects["sphereTex"]->add_transform(translate(mat4(1.0f), lightPosition));

		// Draw Objects in an order here. 
		// alternate the graph to go from shiny to plastic... 
		if(keys[GLFW_KEY_M])
			SetMaterial(prog, shiny, vec3(0.0, 0.7, 0.7));
		else
			SetMaterial(prog, gold, vec3(0.0, 0.7, 0.7));
		objects["graph"]->draw(Model);

		SetMaterial(prog, gold, vec3(0.8, 0.8, 0.0));
		objects["highdummy"]->draw(Model);

		//prog->unbind();
		//solidColorProg->bind();
		
		
		for(size_t i = 0; i < num_dummies; ++i)
		{
			SetMaterial(prog, shiny, vec3((float)i / num_dummies, (float)i / num_dummies, (float)i / num_dummies));
			objects["dummy" + std::to_string(i)]->draw(Model);
		}
		
		SetMaterial(prog, gold, vec3(sin(sTheta), cos(sTheta), sin(sTheta) * cos(sTheta)));
		objects["dummy"]->draw(Model);

		SetMaterial(prog, plastic, vec3(0.5, 0.5, 0.5));
		objects["cloud"]->draw(Model);
		objects["cloud2"]->draw(Model);
		objects["cloud3"]->draw(Model);
		
		prog->unbind();
		texProg->bind();

		SetMaterial(texProg, gold, vec3(0.0,0.2,0));
		texobjects["plane"]->draw(Model);

		SetMaterial(texProg, shiny, vec3(1.0, 1.0, 0.0));
		texobjects["sphereTex"]->draw(Model);

		texProg->unbind();

		//animation update
		sTheta = glfwGetTime();
		cloudsize = 1 + 0.005 * sin((M_E * M_PIf) * glfwGetTime());
		graphT += 0.02;

		

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

		for(std::map<std::string, shared_ptr<Object>>::iterator it = objects.begin(); it != objects.end(); ++it) {
			it->second->flush();
		}
		for(std::map<std::string, shared_ptr<TextureObject>>::iterator it = texobjects.begin(); it != texobjects.end(); ++it) {
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
