#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    m_movementSpeed(SPEED), 
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM),
    m_position(position),
    m_worldUp(up),
    m_yaw(yaw),
    m_pitch(pitch)
{
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    m_movementSpeed(SPEED),
    m_mouseSensitivity(SENSITIVITY), 
    m_zoom(ZOOM)
{
    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

float Camera::getZoom()
{
    return m_zoom;
}

glm::mat4 Camera::getViewMatrix()
{
    //return glm::lookAt(m_position, m_position + m_front, m_up);
    return lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    glm::vec3 cameraDirection = glm::normalize(position - target);
    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::normalize(worldUp), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    
    glm::mat4 translation(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    glm::mat4 rotation(1.0f);
    rotation[0][0] = cameraRight.x;
    rotation[1][0] = cameraRight.y;
    rotation[2][0] = cameraRight.z;

    rotation[0][1] = cameraUp.x;
    rotation[1][1] = cameraUp.y;
    rotation[2][1] = cameraUp.z;

    rotation[0][2] = cameraDirection.x;
    rotation[1][2] = cameraDirection.y;
    rotation[2][2] = cameraDirection.z;

    return rotation * translation;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (direction == FORWARD)
        m_position += m_front * velocity;
    else if (direction == BACKWARD)
        m_position -= m_front * velocity;
    else if (direction == LEFT)
        m_position -= m_right * velocity;
    else if (direction == RIGHT)
        m_position += m_right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yOffset)
{
    m_zoom -= (float) yOffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}