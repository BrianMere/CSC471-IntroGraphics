#include "Object.h"

glm::vec3 Object::getWorldCenterPoint()
{
    MatrixStack M = MatrixStack();
    glm::vec3 relPoint = glm::vec3(0,0,0);

    M.loadIdentity();

    // get the transformations that lead up to this moment from the parent
    // otherwise just use the origin of the world (base case)
    if(this->parentCenter != nullptr)
    {
        relPoint = this->parentCenter->getWorldCenterPoint();
    }

    for (
        auto it = this->transformations.rbegin(); 
        it != this->transformations.rend(); 
        ++it
        ) 
    {
        M.multMatrix(*it);
    }
    for (
        auto it = this->solo_transformations.rbegin(); 
        it != this->solo_transformations.rend(); 
        ++it
        ) 
    {
        M.multMatrix(*it);
    }

    return relPoint + glm::vec3(M.topMatrix() * glm::vec4(this->mid, 1.0f));
}

void Object::draw(std::shared_ptr<MatrixStack> Model)
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
    setModel(*(this->shader), Model);
    this->my_mesh.draw(std::make_shared<Program>(*(this->shader)));

    Model->popMatrix();

}