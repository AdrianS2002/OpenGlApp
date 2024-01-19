#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;

        //TODO - Update the rest of camera parameters
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(glm::normalize(cameraTarget - cameraPosition), cameraUpDirection));

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        if (direction == MOVE_FORWARD) {
            this->cameraPosition += this->cameraFrontDirection * speed;
           
        }
        else if (direction == MOVE_BACKWARD) {
            this->cameraPosition -= this->cameraFrontDirection * speed;
          
        }
        else if (direction == MOVE_LEFT) {
            this->cameraPosition -= this->cameraRightDirection * speed;
        }
        else if (direction == MOVE_RIGHT) {
            this->cameraPosition += this->cameraRightDirection * speed;
         
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        glm::vec3 rot;
        rot.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        rot.y = sin(glm::radians(pitch));
        rot.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(rot);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

}