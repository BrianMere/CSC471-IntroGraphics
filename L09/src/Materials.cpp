#include "Materials.h"

/**
 * helper function to pass material data to the GPU.
 * 
 * Review material_t for supported materials to pass in. 
 * Color is an associated RGB value. 
 * */
void SetMaterial(std::shared_ptr<Program> curS, material_t material, glm::vec3 color) {

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