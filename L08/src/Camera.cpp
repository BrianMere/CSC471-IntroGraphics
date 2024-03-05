#include "Camera.h"

void Camera::recalcViewVector()
{
    // x = rho * sin(phi) * cos(theta)
    this->viewVector.x = this->cameraRot.x * sin(this->cameraRot.z) * cos(this->cameraRot.y);
    // y (math z) = rho * cos(phi)
    this->viewVector.y = this->cameraRot.x * cos(this->cameraRot.z);
    // z (math -y) = rho * sin(phi) * sin(theta) 
    this->viewVector.z = -1 * this->cameraRot.x * sin(this->cameraRot.z) * sin(this->cameraRot.y);
}

void Camera::rotateCamera(double deltaX, double deltaY)
{
    // theta
    this->cameraRot.y += deltaX * this->rotSpeed; 

    // for phi, limit phi to 0 + NECK_DEGS to 180 - NECK_DEGS
    float delta_phi = deltaY * this->rotSpeed;
    if (this->cameraRot.z + delta_phi > M_PIf - this->neckRads)
        this->cameraRot.z = M_PIf - this->neckRads;
    else if (this->cameraRot.z + delta_phi < this->neckRads)
        this->cameraRot.z = this->neckRads;
    else 
        this->cameraRot.z += delta_phi;
}

void Camera::translateCamera(int deltaW, int deltaD)
{   
    this->recalcViewVector();
    glm::vec3 offsetVec = glm::vec3(0.0f, 0.0f, 0.0f);
    offsetVec += deltaW * this->transSpeed * glm::normalize(this->viewVector);
    offsetVec += deltaD * this->transSpeed * glm::normalize(glm::cross(this->viewVector, glm::vec3(0,1,0)));
    this->cameraPos += offsetVec;
}

glm::mat4 Camera::getLookAtMatrix()
{   
    this->recalcViewVector();

    // takes in (eye, target, up)
    return glm::lookAt(
        this->cameraPos, 
        this->cameraPos + this->viewVector, 
        glm::vec3(0,1,0)
    );
}