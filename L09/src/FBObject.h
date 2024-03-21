#ifndef FBO_OBJ_H
#define FBO_OBJ_H

#include <list>

#include "Object.h"
#include "TextureObject.h"
#include "FBO.h"
#include "Camera.h"

class FBObject : public Object
{
private:
    std::shared_ptr<FBO> myFBO;
    glm::vec3 normalVector;
    glm::vec3 startingPoint;

    std::shared_ptr<MatrixStack> prevView;
    std::shared_ptr<MatrixStack> prevModel;
    std::shared_ptr<Program> fboShader; // you want the Object to have the texture shader, not this shader...
public:
    FBObject(
        const std::string &resourceDirectory, 
        const std::string &filename,
        std::shared_ptr<Program> shader,
        std::shared_ptr<Program> fboShader,
        bool isDepth, 
        int width, 
        int height
    ) : Object(resourceDirectory, filename, shader),
        fboShader(fboShader)
    {
        this->myFBO = std::make_shared<FBO>(isDepth,width,height);
        normalVector = glm::vec3(0,0,1);
        startingPoint = glm::vec3(0,0,0);
    };
    FBObject(
        const Object obj, 
        const std::shared_ptr<FBO> i_FBO
    ) :
        Object(obj), 
        myFBO(i_FBO),
        normalVector(0.0,0.0,1.0),
        startingPoint(0,0,0)
    {};
    ~FBObject() {};

    /**
     * Update our FBO with new width and height parameters. 
    */
    void update(int width, int height)
    {
        this->myFBO->init(width, height);
    }

    void draw(std::shared_ptr<MatrixStack>) override;

    /** Clone this Object into new data. */
    std::shared_ptr<FBObject> copy();

    /**
     * The world "Application" that calls this needs to, prior to this call:
     * - Have called this->setupFBO before drawing anything...
     * - Setup any of the relavent transforms for all passed through geometry
     * - Drawn all relavent geometry
     * 
     * This takes in the world camera's view to help reset the binding in the shader. 
    */
    void renderFBO(
        std::shared_ptr<MatrixStack> Model,
        std::vector<std::shared_ptr<Program>> listOfShadersToUpdate
    )
    {
        this->shader->bind();
            this->draw(Model);
        this->shader->bind();

        if(this->prevView != NULL && this->prevModel != NULL)
        {
            for(auto shader : listOfShadersToUpdate)
            {
                shader->bind();
                    CHECKED_GL_CALL(glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, glm::value_ptr(this->prevView->topMatrix())));
                    CHECKED_GL_CALL(glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(this->prevModel->topMatrix())));
                shader->unbind();
            }
            this->prevView = NULL;
            this->prevModel = NULL;
        }
    }

    /**
     * Setup FBO for writing data to:
    */
    void setupFBO(
        std::shared_ptr<MatrixStack> prevView,
        std::shared_ptr<MatrixStack> Model,
        std::vector<std::shared_ptr<Program>> listOfShadersToUpdate
    )
    {
        this->myFBO->ProcessDrawTex(this->shader);

        MatrixStack M = MatrixStack();
        MatrixStack V = MatrixStack();
        M.loadIdentity();
        V.loadIdentity();
    
        Camera c = Camera();

        // Setup the model matrix for this FBObject...
        for (auto it = this->transformations.rbegin(); 
            it != this->transformations.rend(); 
            ++it
        ) {
            M.multMatrix(*it);
        }

        for (auto it = this->solo_transformations.rbegin(); 
            it != this->solo_transformations.rend(); 
            ++it
        ) {
            M.multMatrix(*it);
        }

        c.setPosition(M.topMatrix() * glm::vec4(this->startingPoint, 1));
        c.setRotation(glm::vec3(M.topMatrix() * glm::vec4(this->normalVector, 1.0)));

        V.multMatrix(c.getLookAtMatrix());

        this->prevModel = Model;
        this->prevView = prevView;

        for(auto shader : listOfShadersToUpdate)
        {
            shader->bind();
                CHECKED_GL_CALL(glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, glm::value_ptr(V.topMatrix())));
                CHECKED_GL_CALL(glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M.topMatrix())));
            shader->unbind();
        }
    }

    inline void writeImageFile(const std::string filename)
    {
        this->myFBO->writeImageFile(filename);
    }
};


#endif