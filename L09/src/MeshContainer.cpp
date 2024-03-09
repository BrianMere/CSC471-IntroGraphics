#include "MeshContainer.h"

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

