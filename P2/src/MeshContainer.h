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

    MeshContainer copy() 
    {
        return MeshContainer(this->resourceDirectory, this->filename);
    }
};

/**
 * Create a MeshContainer object. 
 * 
 * resource directory is something like: /usr/bin/
 * filename is the "filename.obj" with the extension.
*/
MeshContainer::MeshContainer(const std::string& resourceDirectory, const std::string& filename)
{
    //EXAMPLE set up to read one shape from one obj file - convert to read several
    // Initialize mesh
    // Load geometry
    // Some obj files contain material information.We'll ignore them for this assignment.

    this->resourceDirectory = resourceDirectory;
    this->filename = filename;
    this->meshes = std::vector<std::shared_ptr<Shape>>();
    bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/" + filename).c_str());

    if (!rc) {
        std::cerr << this->errStr << std::endl;
    } else {
        std::shared_ptr<Shape> newPart;
        //for now all our shapes will not have textures - change in later labs
        for (auto& shape : this->TOshapes)
        {
            newPart = std::make_shared<Shape>(false);
            newPart->createShape(shape);
            newPart->measure();
            newPart->init();

            this->meshes.push_back(newPart);
        }
    }

    //example data that might be useful when trying to compute bounds on multi-shape
	// vec3 gMin;

    // To access mesh bounding box...
    // gMin.x = mesh->min.x;
	// 	gMin.y = mesh->min.y;
}

