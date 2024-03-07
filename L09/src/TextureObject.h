#include "Object.h"
#include "Texture.h"
#include "MatrixStack.h"

class TextureObject : public Object
{
protected:
    std::shared_ptr<Texture> myTex;
public:
    TextureObject(const std::string &resourceDirectory, const std::string &filename,std::shared_ptr<Program> shader, const std::string &texturePictureLocation)
        :   Object(resourceDirectory, filename, shader)
        {
            this->myTex = std::make_shared<Texture>();
            this->myTex->setFilename(resourceDirectory + texturePictureLocation);
            this->myTex->init();
            this->myTex->setUnit(0);
            this->myTex->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        }
    TextureObject(const Object obj, std::shared_ptr<Texture> tex) :
        Object(obj),
        myTex(tex)
        {};

    void draw(std::shared_ptr<MatrixStack>) override;

    /** Clone this Object into new data. */
    std::shared_ptr<TextureObject> copy();
};

/**
 * Note, this method uses the shader's "Texture0" uniform parameter. Make sure you've set this up correctly.
*/
void TextureObject::draw(std::shared_ptr<MatrixStack> Model)
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
    this->myTex->bind(this->shader->getUniform("Texture0"));
    setModel(*(this->shader), Model);
    this->my_mesh.draw(std::make_shared<Program>(*(this->shader)));

    this->myTex->unbind();
    Model->popMatrix();
    
}

std::shared_ptr<TextureObject> TextureObject::copy()
{ 
    std::shared_ptr<TextureObject> n = std::make_shared<TextureObject>(TextureObject(
        *Object::copy(),
        this->myTex
    ));
    return n;
}
