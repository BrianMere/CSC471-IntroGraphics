#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>
#include <glm/glm.hpp>

#ifndef FUNC_GEN_H
#define FUNC_GEN_H


void funcToObj(
    const std::string&, 
    const std::string&,
    const int, 
    const std::function<float (float, float)>,
    const float xmin, 
    const float xmax, 
    const float ymin, 
    const float ymax,
    const std::function<bool (float, float)> = [](float x, float y) {return true;}
);

#endif 

