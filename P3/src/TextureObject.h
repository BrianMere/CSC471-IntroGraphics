#include "Object.h"
#include "Texture.h"
#include "MatrixStack.h"

class TextureObject : public Object
{
protected:
    std::shared_ptr<Texture> myTex;
public:
    TextureObject(const std::string &resourceDirectory, const std::string &filename,std::shared_ptr<Program> shader, std::shared_ptr<Texture> tex)
        :   Object(resourceDirectory, filename, shader),
            myTex(tex) {};

    void draw(std::shared_ptr<MatrixStack>) override;
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
