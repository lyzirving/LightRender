#ifndef LENS_CAMERA_H
#define LENS_CAMERA_H

#include "ICamera.h"

class LensCamera : public ICamera
{
public:
	LensCamera();
	virtual ~LensCamera();

	virtual Ray getRay(float u, float v) override;
private:
	float m_focalDist;
	float m_distLensToSensor;
	float m_distLensToSubject;
	float m_apertureRadius;
};

#endif