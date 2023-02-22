#ifndef LENS_CAMERA_H
#define LENS_CAMERA_H

#include "ICamera.h"

class LensCamera : public ICamera
{
public:
	LensCamera();
	virtual ~LensCamera();
	virtual Ray getRay(float u, float v) override;

	/*
	 * get distance from camera to object plane that could clearly show the object.
	 * this distance goes along the opposite direction of camera front.
	 */
	float clearObjDist();
private:
	float m_focalDist;
	// distance from lens to sensor(image)  
	float m_distI;
	// distance from lens to object(world)
	float m_distO;
	float m_apertureRadius;
};

#endif