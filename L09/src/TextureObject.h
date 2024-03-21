#ifndef MY_TEXTUREOBJ_H
#define MY_TEXTUREOBJ_H

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

#endif
