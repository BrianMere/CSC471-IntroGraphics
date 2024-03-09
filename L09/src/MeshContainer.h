#ifndef MY_MESHCONTAINER_H
#define MY_MESHCONTAINER_H

#include <tiny_obj_loader/tiny_obj_loader.h>
#include <iostream>
#include "Shape.h"

#pragma once

class MeshContainer
{
private:
    std::vector<tinyobj::shape_t> TOshapes;
 	std::vector<tinyobj::material_t> objMaterials;

    std::string resourceDirectory;
    std::string filename;
    std::string errStr;
public:
    // the Shape mesh of this object. Get this whenever. 
    std::vector<std::shared_ptr<Shape>> meshes;
    MeshContainer(const std::string&, const std::string&);
    ~MeshContainer() {};

    // Use the local mesh to draw the object using the passed shader program. 
    inline void draw(std::shared_ptr<Program> prog)
    {
        for(auto mesh : this->meshes)
            mesh->draw(prog);
    }

    // Use this for internal calls to mesh
    inline std::shared_ptr<Shape> getMesh(size_t i) { return this->meshes[i]; };
    inline std::vector<std::shared_ptr<Shape>> getMeshes() { return this->meshes; };

    inline MeshContainer copy() 
    {
        return MeshContainer(this->resourceDirectory, this->filename);
    }
};

#endif
