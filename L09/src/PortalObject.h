#ifndef PORTAL_OBJ_H
#define PORTAL_OBJ_H

#include "function_generator.h"
#include "Bobject.h"
#include "FBObject.h"
#include "particleSys.h"
#include "Texture.h"
#include "TextureObject.h"
#include "Materials.h"

#include <string>
#include <glm/glm.hpp>

/**
 * My own collective implementation of a portal object!
 * 
 * This object includes:
 * - The portal effects (particles)
 * - The portal FBOs and Math to calculate them...
 * - Misc. static textured objs
*/
class PortalObj : public Bobject<>
{
    public:
        PortalObj(
            const std::string& resourceDir, 
            std::shared_ptr<Program> prog,
            glm::vec3 color
        ) : Bobject(resourceDir, prog), 
            color(color)
        {};
        ~PortalObj() {};

        void init() override;
        void init_transforms() override;

        // New method for this object...
        void drawMe(std::shared_ptr<MatrixStack>);

        std::shared_ptr<Program> partProg; // particle shader...

        std::shared_ptr<Program> passProg; // pass through shader for FBOs

        particleSys *thePartSystem;

    private:
        std::shared_ptr<Texture> particleTexture;

        glm::vec3 color;
};

#endif