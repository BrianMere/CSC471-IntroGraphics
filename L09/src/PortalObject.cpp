#include "PortalObject.h"

void PortalObj::init()
{
    // Initialize the GLSL program.
    partProg = make_shared<Program>();
    partProg->setVerbose(true);
    partProg->setShaderNames(
        this->resourceDir + "/particle_vert.glsl",
        this->resourceDir + "/particle_frag.glsl");
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

    // Initialize our FBO shader
    passProg= make_shared<Program>();
    passProg->setVerbose(true);
    passProg->setShaderNames(this->resourceDir + "/pass_vert.glsl", this->resourceDir + "/pass_frag.glsl");
    passProg->init();
    passProg->addUniform("texBuf");
    passProg->addAttribute("vertPos");
    passProg->addUniform("texWidth");
    passProg->addUniform("texHeight");

    funcToObj(
        this->resourceDir + "/portal",
        "portal_plane.obj", 
        1, 
        [](float x, float y) { return 0; },
        -1.0, 
        1.0, 
        -2.0, 
        2.0
    );

    funcToObj(
        this->resourceDir + "/portal",
        "portal_hole.obj", 
        100, 
        [](float x, float y) { return 0; },
        -1.0, 
        1.0, 
        -2.0, 
        2.0,
        [](float x, float y) { return ((pow(x, 2.0f) + pow(y/2.0, 2.0f)) <= 1.0);}
    );

    this->texobjs["PortalFrame"] = make_shared<TextureObject>(TextureObject(
        this->resourceDir + "/portal",
        "portal_plane.obj",
        this->prog, 
        "/portalSwirlGeneric.jpg"
    ));

    // this->texobjs["PlanePortalable"] = make_shared<TextureObject>(TextureObject(
    //     this->resourceDir + "/misc",
    //     "plane.obj",
    //     this->prog, 
    //     "/wall_texture.png"
    // ));

    this->fbobjs["PortalHole"] = make_shared<FBObject>(FBObject(
        this->resourceDir + "/portal", 
        "portal_hole.obj",
        this->passProg,
        this->prog,
        true, 
        640, 
        320
    ));

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
        [this](float t, float a, float b) {
            return this->color + glm::vec3(
                abs(a) * 0.1*t, 
                abs(a) * 0.1*t, 
                abs(a) * 0.1*t
            );
        }
    );
    passProg->bind();
        thePartSystem->gpuSetup();
    passProg->unbind();

    this->particleTexture = make_shared<Texture>(Texture());
    this->particleTexture->setFilename(this->resourceDir +"/particles" + "/alpha.bmp");
    this->particleTexture->init();
    this->particleTexture->setUnit(0);
    this->particleTexture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void PortalObj::init_transforms()
{

    this->texobjs["PortalFrame"]->add_transform(scale(mat4(1.0f), vec3(2.0, 2.0, 2.0)));
    this->texobjs["PortalFrame"]->add_transform(translate(mat4(1.0f), vec3(0,0,0.01)));
    
    // no transformations for PortalHole...

    // update our particle system's origin
	this->thePartSystem->setOrigin(this->texobjs["PortalFrame"]->getWorldCenterPoint());

    std::cout << glm::to_string(this->texobjs["PortalFrame"]->getWorldCenterPoint()) << std::endl;
}

/**
 * Draw this object, with whatever texture is in the FBO currently.
*/
void PortalObj::drawMe(std::shared_ptr<MatrixStack> Model)
{
    partProg->bind();
        this->particleTexture->bind(partProg->getUniform("alphaTexture"));

        thePartSystem->update();
        thePartSystem->drawMe(partProg);
    partProg->unbind();

    this->prog->bind();

        SET_MODE(this->prog, 0); // lighting and shading 

        SetMaterial(this->prog, material_t::plastic, vec3(0.1,0.1,0.1));
        // this->texobjs["PlanePortalable"]->draw(Model);

        SET_MODE(this->prog, 1); // Removing very black pixels from the textures, reserved for portals or other similar textures

        SetMaterial(this->prog, material_t::ethereal, vec3(0,0,0.1));
        this->texobjs["PortalFrame"]->draw(Model);
    this->prog->unbind();
}

