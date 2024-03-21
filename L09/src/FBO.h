#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <cassert>

#include "Program.h"
#include "GLSL.h"
#include "GLTextureWriter.h"

class FBO : Texture
{
private:
    GLuint frameBuf;
    GLuint texBuf;
    GLuint depthBuf; // optional...
    bool isDepth;    // flag to denote if it's okay to use depthBuffer

    GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;

    void initQuad();

public:
    FBO(bool isDepth, int width, int height);
    ~FBO();

    GLuint getFrameBuf() { return frameBuf; };
    GLuint getTexBuf()   { return texBuf;   };
    bool   getIsDepth()  { return isDepth;  };

    void init(int width, int height);

    void ProcessDrawTex(std::shared_ptr<Program> shader, GLuint inTex);
    void ProcessDrawTex(std::shared_ptr<Program> shader){this->ProcessDrawTex(shader, texBuf);};
    void writeImageFile(std::string filename)
    {
        assert(GLTextureWriter::WriteImage(texBuf,filename + ".png"));
    };
};

#endif