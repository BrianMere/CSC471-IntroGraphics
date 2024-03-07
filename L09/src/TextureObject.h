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
    this->myTex->bind(this->shader->getUniform("Texture0"));
    Object::draw(Model);
    this->myTex->unbind();
}

std::shared_ptr<TextureObject> TextureObject::copy()
{ 
    std::shared_ptr<TextureObject> n = std::make_shared<TextureObject>(TextureObject(
        *Object::copy(),
        this->myTex
    ));
    return n;
}
