#ifndef PINHOLE_CAMERA_H
#define PINHOLE_CAMERA_H

#include "ICamera.h"

class PinholeCamera : public ICamera
{
public:
	PinholeCamera();
	virtual ~PinholeCamera();

	virtual Ray getRay(float u, float v) override;
};

#endif
