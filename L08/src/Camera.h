#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "math.h"

class Camera
{
private:
    glm::vec3 cameraPos; // in (x,y,z)
    glm::vec3 cameraRot; // in (r, theta, phi) in radians 

    glm::vec3 viewVector; // view vector in (x,y,z) coords
public:
    float rotSpeed;     // rate at which cameraRot changes per frame (in radians / frame)
    float transSpeed;   // rate at which cameraPos changes per frame
    float neckRads;    // maximum radial angle to have for looking up/down

    Camera() 
    : cameraPos(glm::vec3(0.0f, 0.0f, 0.0f)),
      cameraRot(glm::vec3(1.0f, M_PI_2f, M_PI_2f)), // start looking down the z-axis
      rotSpeed(1.0f), 
      transSpeed(1.0f), 
      neckRads(10.0f * (M_PIf / 180.0f))
    {};
    Camera(
        glm::vec3 initialPosition, 
        glm::vec3 initialRotation,
        float rotSpeed, 
        float transSpeed, 
        float neckRads
    )
    : cameraPos(initialPosition),
      cameraRot(initialRotation), 
      rotSpeed(rotSpeed), 
      transSpeed(transSpeed), 
      neckRads(neckRads)
    {};

    ~Camera() {};

    /**
     * Recalculates the view vector of this object
    */
    void recalcViewVector();

    /**
     * Calculates and applies how much to rotate based on screen input, delta is radians per div
    */
    void rotateCamera(double deltaX, double deltaY);

    /**
     * Translate our camera object in the camera's relative forward position (dollying) via deltaW, and 
     * strafe in the positive deltaD direction. 
    */
    void translateCamera(int deltaW, int deltaD);

    /** 
     * Gets the lookAt Matrix for this Camera
    */
    glm::mat4 getLookAtMatrix();

    /**
     * Reset camera to 0's and default values
    */
    void resetCamera()
    {
      this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
      this->cameraRot = glm::vec3(1.0f, 0.0f, M_PI_2f);
    }

    /**
     * Getter for camera position in (x,y,z)
    */
    inline glm::vec3 getPosition()
    {
      return this->cameraPos;
    }

    /**
     * Sets the camera position to a set value.
     * Useful for teleporting or otherwise strictly controlling the camera.
     * 
    */
    inline void setPosition(glm::vec3 newPos)
    {
      this->cameraPos = newPos;
    }

};

