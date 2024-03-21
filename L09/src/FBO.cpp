#include "FBO.h"

FBO::FBO(bool isDepth, int width, int height)
{
    glGenFramebuffers(1, &frameBuf);
    glGenTextures(1, &texBuf);
    this->isDepth = isDepth;
    if(isDepth)
    {
        glGenRenderbuffers(1, &depthBuf);
    }
    
    if(isDepth)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    }

    this->init(width, height);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    this->initQuad();
}

FBO::~FBO()
{
}

/*
Helper function to create the framebuffer object and associated texture to write to
Previously createFBO
*/
void FBO::init(int width, int height) {

    //set up framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    //set up texture
    glBindTexture(GL_TEXTURE_2D, texBuf);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuf, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Error setting up frame buffer - exiting" << std::endl;
    exit(0);
    }
}

/**
 * Bind this FBO.
 * 
 * This assumes that shader is setup prior to have the "texBuf" uniform, where the
 * passed texture data will be mapped to that uniform. 
 * 
 * If inTex is 0, then we don't bind any texture. This is useful for writing the first time, 
 * without any texture data generated yet.
*/
void FBO::ProcessDrawTex(std::shared_ptr<Program> shader, GLuint inTex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set up inTex as my input texture
    if(inTex != 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTex);
    }
    //example applying of 'drawing' the FBO texture
    //this shader just draws right now
    shader->bind();
      glUniform1i(shader->getUniform("texBuf"), 0); // 0
    //   glEnableVertexAttribArray(0);
    //   glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    //   glDrawArrays(GL_TRIANGLES, 0, 6);
    //   glDisableVertexAttribArray(0);
    shader->unbind();  
}

/**** geometry set up for a quad *****/
void FBO::initQuad() {
    //now set up a simple quad for rendering FBO
        glGenVertexArrays(1, &quad_VertexArrayID);
        glBindVertexArray(quad_VertexArrayID);

        static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

}
