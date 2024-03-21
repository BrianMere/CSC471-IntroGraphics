#ifndef MATERIALS_H
#define MATERIALS_H

#include "Program.h"
#include <glm/glm.hpp>
#include <memory>

#define SET_MODE(texProg, x) (glUniform1f(texProg->getUniform("mode"), x) )
#define SET_SOLIDCOLOR(solidColorProg,x, y, z) (glUniform3f(solidColorProg->getUniform("solidColor"), x, y, z))

typedef enum {
    shiny,
    plastic, 
    metalic,
    gold,
    ethereal
} material_t;

void SetMaterial(std::shared_ptr<Program>, material_t, glm::vec3);

#endif