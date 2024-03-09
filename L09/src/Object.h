#ifndef _OBJECT_H
#define _OBJECT_H

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL // for string functions..

#include "MeshContainer.h"
#include "MatrixStack.h"
#include "Program.h"

#include "GLSL.h"
#include <iostream>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp> // for string casts
#include <functional>


/**
 * Defines a world object that can contain other objects relative to 
 * itself. This is made to abstract away the matrix stack while 
 * still being high in performance. 
*/
class Object : public std::enable_shared_from_this< Object >
{
protected:
    MeshContainer my_mesh;
    std::vector<glm::mat4> transformations;
    std::vector<glm::mat4> solo_transformations;
    glm::mat4 resizeMat; // matrix using the bounding box to resize everything.
    std::shared_ptr<Program> shader;

    glm::vec3 mid; // midpoint in coordinates of the obj, before any coordinates are tranformed

    std::shared_ptr<Object> parentCenter; // Parent relation. Helps to get the parent's relative position in the world.  

   
public:

    std::vector<std::shared_ptr<Object>> sub_objs;

    Object(const std::string &resourceDirectory, const std::string &filename,std::shared_ptr<Program> shader) : 
        my_mesh(MeshContainer(resourceDirectory, filename)), 
        transformations(std::vector<glm::mat4>()),
        solo_transformations(std::vector<glm::mat4>()),
        sub_objs(std::vector<std::shared_ptr<Object>>()),
        shader(shader)
    {
        calcResize();
        parentCenter = nullptr;
    };
    
    ~Object() {}

    /**
     * Sets the relative position of the parent in world space (if this is a subobj). 
    */
    void setParentCenter(std::shared_ptr<Object> o)
    {
        if(o.get() != this)
            this->parentCenter = o;
    }

    /**
     * Calculates and stores this object's resize matrix based on internal data. 
    */
    void calcResize()
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
        this->mid = mid;
        glm::vec3 temp = maxs - mins;
        float largest_extent = glm::max(glm::max(temp.x, temp.y), temp.z);
        largest_extent = 2.0f / largest_extent;

        // We need to:
        // Translate -mid direction
        // Scale down by 2/largest extent (handled above to save cycles)
        this->resizeMat = glm::scale(glm::mat4(1.0f), glm::vec3(largest_extent, largest_extent, largest_extent));
        this->resizeMat = this->resizeMat * glm::translate(glm::mat4(1.0f), mid * -1.0f);

        // lastly, convert mid from obj coordinates (space) to world coordinates (since we always multiply by our resize anyways)
        this->mid = glm::vec3(this->resizeMat * glm::vec4(this->mid, 1.0));
    }

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
     * 
     * Transformations on subobjects subsequently occurs in PARENT space, or in the coordinate space relative to their parent.
    */
    inline void add_subobj(std::shared_ptr<Object> obj)
    {
        this->sub_objs.push_back(obj);
        obj->setParentCenter(shared_from_this());
    }

    /**
     * Draw all objects, and all subobjects, of this object.
     * 
     * It is assumed that Model will change over the course of this call, but on 
     * return it should be back to the same state.  
    */
    virtual void draw(std::shared_ptr<MatrixStack>);

    /* helper for sending top of the matrix strack to GPU */
	void setModel(Program & prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   };

	/* helper function to set model trasnforms */
  	void setModel(Program & curS, glm::vec3 trans, float rotY, float rotX, float sc) {
  		glm::mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
  		glm::mat4 RotX = glm::rotate( glm::mat4(1.0f), rotX, glm::vec3(1, 0, 0));
  		glm::mat4 RotY = glm::rotate( glm::mat4(1.0f), rotY, glm::vec3(0, 1, 0));
  		glm::mat4 ScaleS = glm::scale(glm::mat4(1.0f), glm::vec3(sc));
  		glm::mat4 ctm = Trans*RotX*RotY*ScaleS;
  		glUniformMatrix4fv(curS.getUniform("M"), 1, GL_FALSE, value_ptr(ctm));
  	};

    /** Clone this Object into new data. */
    std::shared_ptr<Object> copy() 
    { 
        std::shared_ptr<Object> n = std::make_shared<Object>(*this); 
        n->my_mesh = this->my_mesh.copy();
        n->solo_transformations.clear();
        // clone copies of each subobject into the new object, as they are pointer based. 
        for(auto obj : this->sub_objs)
        {
            n->sub_objs.push_back(obj->copy());
        }
        n->calcResize();
        
        return n;
    };

    /** Set this Object's shader. */
    inline void setShader(std::shared_ptr<Program> newShader)
    {
        this->shader = newShader;
    }

    /** Copies the transformations of the passed Object to move_to said object 
    */
    inline void move_to(std::shared_ptr<Object> o_obj)
    {   
        this->add_transform(translate(
            glm::mat4(1.0f), 
            o_obj->getWorldCenterPoint() - this->getWorldCenterPoint()
        ));
    }

    /**
     * Move to a point in world space
    */
    inline void move_to(glm::vec3 v)
    {
        this->add_transform(translate(
            glm::mat4(1.0f),
            v - this->getWorldCenterPoint()
        ));
    }

    /**
     * Get the center coordinates of the object currently in world coordinates.
    */
    glm::vec3 getWorldCenterPoint();
   

    /**
     * Print the center point as a glm string on a newline
    */
    inline void printCenter()
    {
        std::cout << glm::to_string(this->getWorldCenterPoint()) << std::endl;
    }
};

#endif