#include "FBObject.h"

/**
 * Note, this method uses the shader's "Texture0" uniform parameter. Make sure you've set this up correctly.
*/
void FBObject::draw(std::shared_ptr<MatrixStack> Model)
{

    Model->pushMatrix();

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
        obj->draw(Model);
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

    // "First" normalize the object. 
    Model->multMatrix(this->resizeMat);

    // Now we can draw our main object. 
    glUniform1i(shader->getUniform("texBuf"), 0);
    setModel(*(this->shader), Model);
    this->my_mesh.draw(std::make_shared<Program>(*(this->shader)));

    Model->popMatrix();
    
}

std::shared_ptr<FBObject> FBObject::copy()
{ 
    std::shared_ptr<FBObject> n = std::make_shared<FBObject>(FBObject(
        *Object::copy(),
        this->myFBO
    ));
    return n;
}