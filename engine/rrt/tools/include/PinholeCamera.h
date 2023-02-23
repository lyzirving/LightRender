#ifndef PINHOLE_CAMERA_H
#define PINHOLE_CAMERA_H

#include "RrtCamera.h"

/*
 * for pinhole camera, near is the focal distance
 */
class PinholeCamera : public RrtCamera
{
public:
	PinholeCamera();
	virtual ~PinholeCamera();

	virtual Ray getRay(float u, float v) override;
};

#endif
