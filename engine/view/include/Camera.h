#ifndef CAMERA_H
#define CAMERA_H

#include <atomic>
#include <glm/glm.hpp>

class Camera
{
public:

    Camera(float camTheta = 90.f, float camPhi = 0.f, float camRadius = 5.f, float viewDirTheta = 90.f, float viewDirPhi = 180.f);
    virtual ~Camera();

    inline bool isChanged() { return m_dataChange.load(); }

    const glm::mat4& getViewMat();
    const glm::vec3& getCamPos();

    void setPosition(float theta, float phi, float r);

private:
    void calcViewMat();
    void calcViewDirection();
    void calcCameraPosition();
    void checkCamPosPhi();
    void checkCamPos();
    void keepViewDirToOrigin();

    glm::vec3 m_worldUp;
    glm::vec3 m_camPos;

    // +x axis in camera local coordinate
    glm::vec3 m_camRight;
    // +y axis in camera local coordinate
    glm::vec3 m_camUp;
    // -z axis in camera local coordinate
    glm::vec3 m_viewDir;

    // (theta, phi, r) spherical coordinate based on right handed coordinate system.
    // theta is polar angle between +y axis and vector OC, while O is origin, C is camera position.
    float m_camPosTheta;

    // (theta, phi, r) spherical coordinate based on right handed coordinate system.
    // phi is azimuth between +z and projected vector of camera position on plane x-z.
    float m_camPosPhi;

    // (theta, phi, r) spherical coordinate based on right handed coordinate system.
    // r specifies camera position's absolute length
    float m_camPosRadius;

    // spherical coordinate of view sight direction. r for view direction is 1.0.
    float m_viewDirTheta, m_viewDirPhi;

    glm::mat4 m_viewMat;

    std::atomic_bool m_dataChange;
};

#endif // !CAMERA_H

