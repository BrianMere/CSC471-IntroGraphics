#include "MeshContainer.h"
#include "MatrixStack.h"
#include "Program.h"

#include "GLSL.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

/**
 * Defines a world object that can contain other objects relative to 
 * itself. This is made to abstract away the matrix stack while 
 * still being high in performance. 
*/
class Object
{
private:
    MeshContainer my_mesh;
    std::vector<glm::mat4> transformations;
    std::vector<glm::mat4> solo_transformations;
    glm::mat4 resizeMat; // matrix using the bounding box to resize everything.
public:

    std::vector<std::shared_ptr<Object>> sub_objs;

    Object(const std::string &resourceDirectory, const std::string &filename) : 
        my_mesh(MeshContainer(resourceDirectory, filename)), 
        transformations(std::vector<glm::mat4>()),
        solo_transformations(std::vector<glm::mat4>()),
        sub_objs(std::vector<std::shared_ptr<Object>>())
    {
        // calculate bounding box and store internally
        glm::vec3 mins, maxs;
        mins = glm::vec3(std::numeric_limits<float>::max());
        maxs = glm::vec3(std::numeric_limits<float>::min());
        for(auto mesh : this->my_mesh.getMeshes())
        {
            mins = glm::min(mins, mesh->min);
            maxs = glm::max(maxs, mesh->max);
        }

        glm::vec3 mid = maxs * 0.5f + mins * 0.5f;
        glm::vec3 temp = maxs - mins;
        float largest_extent = glm::max(glm::max(temp.x, temp.y), temp.z);
        largest_extent = 2.0f / largest_extent;

        // We need to:
        // Translate -mid direction
        // Scale down by 2/largest extent (handled above to save cycles)
        this->resizeMat = glm::scale(glm::mat4(1.0f), glm::vec3(largest_extent, largest_extent, largest_extent));
        this->resizeMat = this->resizeMat * glm::translate(glm::mat4(1.0f), mid * -1.0f);
    };
    ~Object() {}

    /**
     * Add a transform from the MatrixStack to a local tracker of transformations in order.
     * 
     * This affects the transformations of child objects.  
    */
    inline void add_transform(glm::mat4 matrix)
    {
        this->transformations.push_back(matrix);
    }
    
    /**
     * Add a transform that ONLY affects this own object. 
     * 
     * Note that these are transformations that happen AFTER we draw our subobjects. 
    */
    inline void add_solo_transform(glm::mat4 matrix)
    {
        this->solo_transformations.push_back(matrix);
    }
    
    /**
     * Remove the log of this object's transforms, subobjs, and everything
    */
    inline void flush() 
    {
        this->transformations.clear();
        this->solo_transformations.clear();
        this->sub_objs.clear();
    }

    /**
     * Add an object as a child with transformations relative to this object
    */
    inline void add_subobj(std::shared_ptr<Object> obj)
    {
        this->sub_objs.push_back(obj);
    }

    

    /**
     * Draw all objects, and all subobjects, of this object.
     * 
     * It is assumed that Model will change over the course of this call, but on 
     * return it should be back to the same state.  
    */
    void draw(std::shared_ptr<MatrixStack>, std::shared_ptr<Program>);

    /* helper for sending top of the matrix strack to GPU */
	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   };

	/* helper function to set model trasnforms */
  	void setModel(std::shared_ptr<Program> curS, glm::vec3 trans, float rotY, float rotX, float sc) {
  		glm::mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
  		glm::mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, glm::vec3(1, 0, 0));
  		glm::mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, glm::vec3(0, 1, 0));
  		glm::mat4 ScaleS = glm::scale(glm::mat4(1.0f), glm::vec3(sc));
  		glm::mat4 ctm = Trans*RotX*RotY*ScaleS;
  		glUniformMatrix4fv(curS->getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
  	};

    /** Clone this Object into new data. */
    std::shared_ptr<Object> copy() 
    { 
        std::shared_ptr<Object> n = std::make_shared<Object>(*this); 
        n->flush();
        return n;
    };
};

void Object::draw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> prog)
{

    Model->pushMatrix();

    // First normalize the object. 
    Model->multMatrix(this->resizeMat);

    // Put all our transformations into Model (need to go reverse of the order we added)
    for (
        auto it = this->transformations.rbegin(); 
        it != this->transformations.rend(); 
        ++it
    ) {
        Model->multMatrix(*it);
    }

    // Draw all of our subobjects
    Model->pushMatrix();
    for(auto obj : this->sub_objs)
    {
        obj->draw(Model, prog);
    }
    Model->popMatrix();

    // do our post transformations (same idea as above)
    for (
        auto it = this->solo_transformations.rbegin(); 
        it != this->solo_transformations.rend(); 
        ++it
    ) {
        Model->multMatrix(*it);
    }

    // Now we can draw our main object. 
    setModel(prog, Model);
    this->my_mesh.draw(prog);

    Model->popMatrix();

}
